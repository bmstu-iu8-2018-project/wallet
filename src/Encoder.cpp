#include <includes/Encoder.hpp>

std::string Encoder::encrypt(const std::string& text, const std::string& key) {
  EVP_CIPHER_CTX en;
  Encoder e;
  e.aes_init_enc(key, &en);
  auto ciphertext = e.aes_encrypt(&en, text);
  EVP_CIPHER_CTX_cleanup(&en);
  std::string hex_enc;
  for (const auto i : ciphertext)
    hex_enc += e.hexString(i);
  return hex_enc;
}

std::string Encoder::decrypt(const std::string& ciphertext,
                             const std::string& key) {
  EVP_CIPHER_CTX de;
  Encoder e;
  auto ct = e.unHexString(ciphertext);
  e.aes_init_dec(key, &de);
  auto plaintext = e.aes_decrypt(&de, ct);
  EVP_CIPHER_CTX_cleanup(&de);
  return plaintext;
}

void Encoder::aes_init_dec(const std::string& key, EVP_CIPHER_CTX* d_ctx) {
  unsigned int salt[] = {12345, 54321};
  unsigned char key_b[32], iv[32];

  const unsigned char* key_data =
      reinterpret_cast<const unsigned char*>(key.c_str());

  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(),
                         reinterpret_cast<unsigned char*>(&salt), key_data,
                         key.size() + 1, 5, key_b, iv);

  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key_b, iv);
}

void Encoder::aes_init_enc(const std::string& key, EVP_CIPHER_CTX* e_ctx) {
  unsigned int salt[] = {12345, 54321};
  unsigned char key_b[32], iv[32];

  const unsigned char* key_data =
      reinterpret_cast<const unsigned char*>(key.c_str());

  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(),
                         reinterpret_cast<unsigned char*>(&salt), key_data,
                         key.size() + 1, 5, key_b, iv);

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key_b, iv);
}

std::string Encoder::aes_encrypt(EVP_CIPHER_CTX* e,
                                 const std::string& plaintext) {
  int c_len = plaintext.size() + AES_BLOCK_SIZE + 1;
  int f_len = 0;
  unsigned char* ciphertext = static_cast<unsigned char*>(malloc(c_len));

  const unsigned char* plain =
      reinterpret_cast<const unsigned char*>(plaintext.c_str());

  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_EncryptUpdate(e, ciphertext, &c_len, plain, plaintext.size() + 1);
  EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

  std::string res(reinterpret_cast<char*>(ciphertext));
  free(ciphertext);

  return res;
}

std::string Encoder::aes_decrypt(EVP_CIPHER_CTX* e,
                                 const std::string& ciphertext) {
  const unsigned char* cipher =
      reinterpret_cast<const unsigned char*>(ciphertext.c_str());

  int p_len = ciphertext.size();
  int f_len = 0;
  unsigned char* plaintext = static_cast<unsigned char*>(malloc(p_len));

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, cipher, ciphertext.size() + 1);
  EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

  std::string res(reinterpret_cast<char*>(plaintext));
  free(plaintext);

  return res;
}

std::string Encoder::hexString(unsigned char ch) {
  std::string hexStr;
  std::stringstream sstream;
  sstream << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
          << (int)ch;
  hexStr = sstream.str();
  sstream.clear();
  return hexStr;
}

std::string Encoder::unHexString(const std::string& mess) {
  std::string UnHexString;
  unsigned short dec;
  std::string tmp;
  for (int i = 0; i < mess.size(); i = i + 2) {
    tmp.push_back(mess[i]);
    tmp.push_back(mess[i + 1]);
    std::istringstream(tmp) >> std::hex >> dec;
    UnHexString.push_back(dec);
    tmp.clear();
  }
  return UnHexString;
}
