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

    void create_wallet(const byte type_network);

    std::string get_private_key() const noexcept;

    std::string get_public_key() const noexcept;

    std::string get_address() const noexcept;
};
#endif
