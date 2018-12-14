#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <algorithm>
#include <fstream>
#include <includes/Script.hpp>
#include <includes/TxIn.hpp>
#include <includes/TxOut.hpp>
#include <string>

using outputs = std::vector<TxOut>;
using inputs = std::vector<TxIn>;

class Transaction {
 public:
  Transaction() = default;

  ~Transaction() = default;

  Transaction(const std::string& file_name);

  Transaction(int32_t version,
              const inputs& input,
              const outputs& output,
              uint32_t lock_time);

  Transaction(Transaction&& other);

  Transaction(const Transaction& other);

  Transaction& operator=(Transaction&& other);

  Transaction& operator=(const Transaction& other);

  std::string get_hex_tx() const;

  std::vector<byte> get_byte_tx() const;

  void sign(const std::string& private_key_wif);

  bool is_signed() const;

  static Transaction parse(const std::string& file_name);

  std::vector<TxIn> get_inputs() const;

  std::vector<TxOut> get_outputs() const;

  int32_t get_version() const;

  uint32_t get_locktime() const;

  byte get_in_count() const;

  byte get_out_count() const;

 private:
  int32_t version_;

  byte tx_in_count_;
  inputs tx_in_;

  byte tx_out_count_;
  outputs tx_out_;

  uint32_t locktime_;
};

#endif  // TRANSACTION_HPP
