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
#include <array>
#include <chrono>

CryptoPP::AutoSeededRandomPool prng;
std::array<std::string, 2> encrypt(std::string plain)
{
    auto cur = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    for (auto i = 0; i < key.size(); i++)
    {
        key.begin()[i] = static_cast<CryptoPP::byte>(cur % (256 - i));
    }

    CryptoPP::byte iv_bytes[CryptoPP::AES::BLOCKSIZE];
    prng.GenerateBlock(iv_bytes, sizeof(iv_bytes));

    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;
    e.SetKeyWithIV(key, key.size(), iv_bytes);

    std::string output_bytes;
    CryptoPP::StringSource ss(plain, true,
                              new CryptoPP::StreamTransformationFilter(e,
                                                                       new CryptoPP::StringSink(output_bytes)) // StreamTransformationFilter
    );

    std::string output;
    CryptoPP::StringSource ss_key(output_bytes, true,
                              new CryptoPP::HexEncoder(
                                  new CryptoPP::StringSink(output)));

    std::string iv;
    std::string iv_bytes_str{reinterpret_cast<const char *>(iv_bytes), sizeof(iv_bytes)};
    CryptoPP::StringSource ss_iv(iv_bytes_str, true,
                              new CryptoPP::HexEncoder(
                                  new CryptoPP::StringSink(iv)));
    return {output, iv};
}

EMSCRIPTEN_BINDINGS(Module)
{
    emscripten::value_array<std::array<std::string, 2>>("array_string_2")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>());
    emscripten::function("encrypt", &encrypt);
}