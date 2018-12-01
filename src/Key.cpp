#include <includes/Key.hpp>

void Key::make_address(const byte type_network)
{
    std::string address = cu::to_hex(type_network) + cu::RIPEMD160(cu::SHA256(get_public_key()));

    auto checksum = cu::SHA256(cu::SHA256(address));
    address.insert(address.end(), checksum.begin(), checksum.begin() + 8);

    address_ = cu::to_base58(cu::from_hex_to_bytes(address));
};

std::string Key::private_to_wif(const std::string& key)
{
    std::string private_key = cu::to_hex(PRIVATE_PREFIX) + key;

    auto checksum = cu::SHA256(cu::SHA256(private_key));
    private_key.insert(private_key.end(), checksum.begin(), checksum.begin() + 8);

    return cu::to_base58(cu::from_hex_to_bytes(private_key));
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
    keys_ = std::make_pair(wif_privet_key, public_key);

    EC_KEY_free(ec_key);
    BN_CTX_free(ctx);
};

std::string Key::get_public_key() const noexcept
{
    return keys_.second;
};

std::string Key::get_private_key() const noexcept
{
    return keys_.first;
};

std::string Key::get_address() const noexcept
{
    return address_;
};

Key Key::create_wallet_key(const byte type_network)
{
    Key key;
    key.generate_keys();
    key.make_address(type_network);
    return key;
};
