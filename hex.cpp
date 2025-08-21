#include <iostream>
#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>

std::string calculateFileHash(const std::string& filePath) {
    std::string digest;
    CryptoPP::SHA256 hash;

    try {
        CryptoPP::FileSource fileSource(
            filePath.c_str(),
            true,
            new CryptoPP::HashFilter(
                hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // lowercase hex
                )
            )
        );
    } catch (const std::exception& e) {
        std::cerr << "Error reading/hashing file: " 
                  << filePath << " - " << e.what() << std::endl;
        return "";
    }
    return digest;
}
