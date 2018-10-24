#pragma once
#ifndef KEY_H
#define KEY_H

#include "CryptoUtils.h"

using namespace cu;

class Key
{
 private:
    std::pair<std::string, std::string> _Keys;
    std::string _Address;

    void make_address()
    {
        std::string hash = "00"; // основная сеть 0x00, тестовая сеть 0х0f, Namecoin сеть 0х34;
        hash += RIPEMD160(SHA256(get_public_key()));

        auto checksum = SHA256(SHA256(hash));
        checksum.erase(checksum.begin() + 8, checksum.end());
        hash += checksum;

        std::vector<byte> bytes = from_hex_to_bytes(hash);

        _Address = to_base58(bytes);
    }

    std::string private_to_wif(const std::string& key)
    {
        std::string private_key = "80";
        private_key += key;
        std::vector<byte> bytes = from_hex_to_bytes(private_key);
        return to_base58(bytes);
    }

 public:
    Key() = default;
    ~Key() = default;

    void generate_keys()
    {
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);

        if (!ec_key)
            throw std::runtime_error("Error creating curve key");

        if (!EC_KEY_generate_key(ec_key))
            throw std::runtime_error("Error creating curve key");

        BIGNUM const* prv = EC_KEY_get0_private_key(ec_key);

        if (!prv)
            throw std::runtime_error("Error creating curve key");

        EC_POINT const* pub = EC_KEY_get0_public_key(ec_key);
        if (!pub)
            throw std::runtime_error("Error creating curve key");

        const EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
        BN_CTX* ctx = BN_CTX_new();

        char* public_key = EC_POINT_point2hex(group, pub, POINT_CONVERSION_UNCOMPRESSED, ctx); // uncompressed format 65 bit: 32 bit for x, 32 for y, 1 bit format
        char* private_key = BN_bn2hex(prv);

        auto wif_privet_key = private_to_wif(private_key);
        _Keys = std::make_pair(wif_privet_key, public_key);

        EC_KEY_free(ec_key);
        BN_CTX_free(ctx);
    }

    std::string get_public_key() const noexcept
    {
        return _Keys.second;
    }

    std::string get_private_key() const noexcept
    {
        return _Keys.first;
    }

    std::string get_address() const noexcept
    {
        return _Address;
    }

    static Key create_wallet_key()
    {
        Key key;
        key.generate_keys();
        key.make_address();
        return key;
    }
};
#endif
