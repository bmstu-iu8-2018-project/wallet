#pragma once
#ifndef CRYPTO_H
#define CRYPTO_H

#include <exception>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>

using byte = unsigned char;

std::vector<byte> from_hex_to_bytes(const std::string& hex)
{
    std::vector<byte> vector_of_bytes;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        byte ibyte = static_cast<byte>(strtol(byteString.c_str(), NULL, 16));
        vector_of_bytes.push_back(ibyte);
    }
    return vector_of_bytes;
}

std::string to_hex(unsigned char s)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s);

    return ss.str();
};

std::string SHA256(const std::string& string)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string.c_str(), string.length());
    SHA256_Final(hash, &sha256);

    std::string output;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        output += to_hex(hash[i]);

    return output;
};

std::string RIPEMD160(const std::string& string)
{
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ripemd160;
    RIPEMD160_Init(&ripemd160);
    RIPEMD160_Update(&ripemd160, string.c_str(), string.length());
    RIPEMD160_Final(hash, &ripemd160);

    std::string output;
    for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
        output += to_hex(hash[i]);

    return  output;
};

#endif
