#pragma once
#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <cassert>
#include <cstring>
#include <exception>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>


using byte = unsigned char;

namespace cu
{
    static const char* Base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::vector<byte> from_hex_to_bytes(const std::string& hex);

    std::string from_bytes_to_hex(const std::vector<byte>& bytes);
    
    std::string to_hex(byte s);

    std::string SHA256(const std::string& string);

    std::string RIPEMD160(const std::string& string);

    std::string to_base58(const unsigned char* pbegin, const unsigned char* pend);

    std::string to_base58(const std::vector<byte>& vch);

    bool from_base58(const char *psz, std::vector<byte>& vch);

    bool from_base58(const std::string& str, std::vector<byte>& vchRet);

    BIGNUM* get_private_key(const std::string& private_key);

    EC_KEY* get_ec_key_from_private(const std::string& private_key);

    EC_KEY* get_ec_key_from_public(const std::string& public_key);

    ECDSA_SIG* sign(const std::string& private_key, const std::string& text);

    bool is_validate_signature(const std::string& public_key, const ECDSA_SIG* signature, const std::string& text);
}
#endif
