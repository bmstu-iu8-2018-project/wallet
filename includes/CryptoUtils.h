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
using const_bytes = const unsigned char*;
const int DIGEST_LENGTH = 32;
const byte DER_HEADLINE = 0x30;
const byte BEGIN_OF_NUM = 0x02;
const byte SIGHASH_ALL = 0x01;  // signature is valid for all exits


namespace cu
{
    static const char* Base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::vector<byte> from_hex_to_bytes(const std::string& hex);

    std::string from_bytes_to_hex(const_bytes bytes, int length);

    std::string from_bytes_to_hex(const std::vector<byte>& bytes);

    std::string to_hex(byte s);

    std::string SHA256(const std::string& string);

    std::string RIPEMD160(const std::string& string);

    std::string to_base58(const_bytes pbegin, const_bytes pend);

    std::string to_base58(const std::vector<byte>& vch);

    bool from_base58(const char *psz, std::vector<byte>& vch);

    bool from_base58(const std::string& str, std::vector<byte>& vchRet);

    BIGNUM* get_private_key(const std::string& private_key);

    EC_KEY* get_ec_key_from_private(const std::string& private_key);

    EC_KEY* get_ec_key_from_public(const std::string& public_key);

    ECDSA_SIG* sign(const std::string& private_key, const std::string& text);

    bool is_validate_signature(const std::string& public_key, const ECDSA_SIG* signature, const std::string& text);

    std::string signature_to_der(ECDSA_SIG* signature);

    ECDSA_SIG* from_der_to_sig(const std::string& scriptSig);
}
#endif
