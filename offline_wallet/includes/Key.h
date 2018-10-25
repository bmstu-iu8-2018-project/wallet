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

    void make_address();

    std::string private_to_wif(const std::string& key);
 public:
    Key() = default;

    ~Key() = default;

    void generate_keys();

    std::string get_public_key() const noexcept;

    std::string get_private_key() const noexcept;

    std::string get_address() const noexcept;

    static Key create_wallet_key();
};
#endif