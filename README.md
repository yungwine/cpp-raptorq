
# RaptorQ (RFC6330) c++ bindings

## The whole code except `raptorq/` folder has been taken from [TON Blockchain](https://github.com/ton-blockchain/ton) repo


## Compile from sources

```bash
git clone https://github.com/yungwine/cpp-raptorq.git
cd cpp-raptorq
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

The shared library binary file will be at `./raptorq/libpyraptorq.so`
