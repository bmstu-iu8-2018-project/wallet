#ifndef ENCODER_HPP
#define ENCODER_HPP
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <includes/CryptoUtils.hpp>
#include <iostream>
#include <string>
#include <vector>

class Encoder {
 public:
  Encoder() = default;

  ~Encoder() = default;

  static std::string encrypt(const std::string& text, const std::string& key);

  static std::string decrypt(const std::string& ciphertext,
                             const std::string& key);

 private:
  void aes_init_dec(const std::string& key, EVP_CIPHER_CTX* d_ctx);

  void aes_init_enc(const std::string& key, EVP_CIPHER_CTX* e_ctx);

  std::string aes_encrypt(EVP_CIPHER_CTX* e, const std::string& plaintext);

  std::string aes_decrypt(EVP_CIPHER_CTX* e, const std::string& ciphertext);

  std::string hexString(unsigned char ch);

  std::string unHexString(const std::string& mess);
};

#endif  // ENCODER_HPP
