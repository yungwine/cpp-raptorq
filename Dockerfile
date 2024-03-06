FROM ubuntu:18.04

WORKDIR /code

COPY . .

RUN apt-get update

RUN apt-get install -y \
        make \
        build-essential \
        cmake \
        pkg-config \
    zlib1g-dev \
    libsecp256k1-dev \
    libmicrohttpd-dev \
    libsodium-dev

RUN mkdir build
RUN cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
RUN cd build && make
RUN ls build
RUN ls build/raptorq

