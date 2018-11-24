#pragma once
#ifndef KEY_HPP
#define KEY_HPP

#include <includes/CryptoUtils.hpp>

const byte MAIN_NETWORK = 0x00;
const byte TEST_NETWORK = 0x6f;
const byte NAMECOIN_NETWORK = 0x34;
const byte PRIVATE_PREFIX = 0x80;

using namespace cu;

class Key
{
 private:
    std::pair<std::string, std::string> _Keys;
    std::string _Address;

    void make_address(const byte type_network);

    std::string private_to_wif(const std::string& key);
 public:
    Key() = default;

    ~Key() = default;

    void generate_keys();

    std::string get_public_key() const noexcept;

    std::string get_private_key() const noexcept;

    std::string get_address() const noexcept;

    static Key create_wallet_key(const byte type_network);
};

#endif // KEY_HPP
