//
// Created by Maksim Kurbatov on 23.02.2024.
//


#include "tdfec/td/fec/raptorq/Decoder.h"
#include "tdfec/td/fec/raptorq/Encoder.h"

#include "td/utils/base64.h"


static TD_THREAD_LOCAL std::string *current_output;


static td::CSlice store_string(std::string str) {
  td::init_thread_local<std::string>(current_output);
  *current_output = std::move(str);
  return *current_output;
}


extern "C" {

int test_link(int a, int b) {
  return a + b;
}

void *get_decoder(int symbols_count, int symbols_size, int data_size) {
  td::raptorq::Encoder::Parameters p{static_cast<size_t>(symbols_count), static_cast<size_t>(symbols_size),
                                     static_cast<size_t>(data_size)};
  auto decoder = td::raptorq::Decoder::create(p).move_as_ok();
  auto rfc_p = td::raptorq::Rfc::get_parameters(p.symbols_count);
  return new td::raptorq::Decoder(rfc_p.move_as_ok(), p.symbol_size, p.data_size);
}

bool may_try_decode(void *decoder) {
  return static_cast<td::raptorq::Decoder *>(decoder)->may_try_decode();
}

bool add_symbol(void *decoder, int id, int data_size, const char *data) {
  auto sl = td::Slice(data, data_size);
  td::raptorq::SymbolRef symbol{static_cast<td::uint32>(id), sl};
  td::Status res = static_cast<td::raptorq::Decoder *>(decoder)->add_symbol(symbol);
  return res.is_ok();
}


const char *try_decode(void *decoder) {
  auto res = static_cast<td::raptorq::Decoder *>(decoder)->try_decode(true);
  if (res.is_error()) {
    return nullptr;
  }
  auto data = res.move_as_ok().data;
  auto b64enc = td::base64_encode(data);

  return store_string(std::move(b64enc)).c_str(); // because otherwise the string will be cutten by first \x00 byte
}

void destroy_decoder(void *decoder) {
  delete static_cast<td::raptorq::Decoder *>(decoder);
}


void *get_encoder(const char *data, int data_size, int symbol_size) {
  auto p = td::raptorq::Rfc::get_parameters((data_size + symbol_size - 1) / symbol_size);
  if (p.is_error()) {
    return nullptr;
  }

  auto slice = td::BufferSlice(data, data_size);
  auto encoder = new td::raptorq::Encoder(p.move_as_ok(), static_cast<size_t>(symbol_size), std::move(slice));
  encoder->precalc();
  if (!encoder->has_precalc()) {
    return nullptr; // probably errors happen
  }
  return encoder;
}

const char *gen_symbol(void *encoder, int id) {

  auto encoderc = static_cast<td::raptorq::Encoder *>(encoder);
  td::BufferSlice data(encoderc->get_parameters().symbol_size);
  auto res = encoderc->gen_symbol(id, data.as_slice());
  if (res.is_error()) {
    return nullptr;
  }
  auto b64enc = td::base64_encode(data);
  return store_string(std::move(b64enc)).c_str();
}

void destroy_encoder(void *encoder) {

  delete static_cast<td::raptorq::Encoder *>(encoder);
}

}

