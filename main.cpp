#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include <cryptopp/cryptlib.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/seed.h>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <string>
#include <sstream>
#include <iostream>

std::string aes_cbc_mode_encrypt(std::string &plain, CryptoPP::SecByteBlock key, CryptoPP::byte *iv) {
    std::string cipher;
    std::string output;

    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e(key, key.size(), iv);

        CryptoPP::StringSource(plain, true,
            new CryptoPP::StreamTransformationFilter(e,
                new CryptoPP::StringSink(cipher)
            ) //StreamTransformationFilter
        ); // StringSource
    } catch (CryptoPP::Exception &exception) {
        std::cerr << exception.what() << std::endl;
        exit(1);
    }

    CryptoPP::StringSource(cipher, true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(output)
        ) // HexEncoder
    ); // StringSource
    return output;
}

std::string encrypt(std::string data){
    unsigned char key_str[16] = "hello world";
    char iv[16] = "hello world an";
    CryptoPP::SecByteBlock key(reinterpret_cast<const unsigned char*>(key_str), sizeof(key_str));
    auto result = aes_cbc_mode_encrypt(data, key, reinterpret_cast<unsigned char*>(iv));
    return result;
}


EMSCRIPTEN_BINDINGS(Module)
{
   emscripten::function("encrypt", &encrypt);
}