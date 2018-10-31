#include "Key.h"

void Key::make_address()
{
    std::string address = "00"; // main network 0x00, test network 0x0f, Namecoin network 0x34;
    address += RIPEMD160(SHA256(get_public_key()));

    auto checksum = SHA256(SHA256(address));
    address.insert(address.end(), checksum.begin(), checksum.begin() + 8);

    _Address = to_base58(from_hex_to_bytes(address));
};

std::string Key::private_to_wif(const std::string& key)
{
    std::string private_key = "80";
    private_key += key;

    auto checksum = SHA256(SHA256(private_key));
    private_key.insert(private_key.end(), checksum.begin(), checksum.begin() + 8);

    return to_base58(from_hex_to_bytes(private_key));
};

void Key::generate_keys()
{
    EC_KEY * ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ec_key)
        throw std::runtime_error("Error creating curve key");

    if (!EC_KEY_generate_key(ec_key))
        throw std::runtime_error("Error creating curve key");

    const BIGNUM* prv = EC_KEY_get0_private_key(ec_key);

    if (!prv)
        throw std::runtime_error("Error creating curve key");

    const EC_POINT* pub = EC_KEY_get0_public_key(ec_key);
    if (!pub)
        throw std::runtime_error("Error creating curve key");

    BN_CTX* ctx = BN_CTX_new();
    const EC_GROUP* group = EC_KEY_get0_group(ec_key);

    char* public_key = EC_POINT_point2hex(group, pub, POINT_CONVERSION_UNCOMPRESSED, ctx); // uncompressed format 65 bit: 32 bit for x, 32 for y, 1 bit format
    char* private_key = BN_bn2hex(prv);

    auto wif_privet_key = private_to_wif(private_key);
    _Keys = std::make_pair(wif_privet_key, public_key);

    EC_KEY_free(ec_key);
    BN_CTX_free(ctx);
};

std::string Key::get_public_key() const noexcept
{
    return _Keys.second;
};

std::string Key::get_private_key() const noexcept
{
    return _Keys.first;
};

std::string Key::get_address() const noexcept
{
    return _Address;
};

Key Key::create_wallet_key()
{
    Key key;
    key.generate_keys();
    key.make_address();
    return key;
};
