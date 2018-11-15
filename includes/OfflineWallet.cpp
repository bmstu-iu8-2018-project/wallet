#include "OfflineWallet.h"

void OfflineWallet::create_wallet(const byte type_network)
{
    _Keys = Key::create_wallet_key(type_network);
}

std::string OfflineWallet::get_private_key() const noexcept
{
    return _Keys.get_private_key();
}

std::string OfflineWallet::get_public_key() const noexcept
{
    return _Keys.get_public_key();
}

std::string OfflineWallet::get_address() const noexcept
{
    return _Keys.get_address();
}
