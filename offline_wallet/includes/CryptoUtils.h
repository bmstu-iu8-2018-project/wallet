#pragma once
#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <cassert>
#include <exception>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>

using byte = unsigned char;


namespace cu
{
    static const char* Base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::vector<byte> from_hex_to_bytes(const std::string& hex);
    
    std::string to_hex(unsigned char s);

    std::string SHA256(const std::string& string);

    std::string RIPEMD160(const std::string& string);

    std::string to_base58(const unsigned char* pbegin, const unsigned char* pend);

    std::string to_base58(const std::vector<unsigned char>& vch);
}
#endif
