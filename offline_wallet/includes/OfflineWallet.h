#pragma once
#ifndef OFFLINE_WALLET_H
#define OFFLINE_WALLET_H

#include "Key.h"

class OfflineWallet
{
private:
    Key _Keys;
public:
    OfflineWallet() = default;
    ~OfflineWallet() = default;

    void create_wallet()
    {
        _Keys = Key::create_wallet_key();
        // save keys ...
    }

    std::string get_private_key() const noexcept
    {
        return _Keys.get_private_key();
    }

    std::string get_public_key() const noexcept
    {
        return _Keys.get_public_key();
    }

    std::string get_address() const noexcept
    {
        return _Keys.get_address();
    }
};

#endif
