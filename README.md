# Kies-XSource-WebAssembly

This project aims to provide a webassembly implemented function in C++ to JavaScript in XSource project.

It's now a pretty small code and only get my hands dirty on WASM.

This code use `Vcpkg` to install dependency and `Emscripten` to compile the code to WebAssembly. The only dependency is `Crypto++` which used to encrypt the message sent from browser.

## Usage

1. Clone emsdk and active it.

2. Clone the vcpkg and active it.

3. Type `./vcpkg install cryptopp:wasm32-emscripten`

4. Just compile it with `CMake`. All the things are handled by toolchain files.