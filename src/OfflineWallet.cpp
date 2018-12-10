#include <includes/OfflineWallet.hpp>

void OfflineWallet::create_wallet(const byte type_network) {
  keys_ = Key::create_wallet_key(type_network);
}

std::string OfflineWallet::get_private_key() const noexcept {
  return keys_.get_private_key();
}

std::string OfflineWallet::get_public_key() const noexcept {
  return keys_.get_public_key();
}

std::string OfflineWallet::get_address() const noexcept {
  return keys_.get_address();
}
