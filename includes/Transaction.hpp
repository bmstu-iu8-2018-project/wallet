#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <algorithm>
#include <fstream>
#include <string>
#include <includes/Script.hpp>
#include <includes/TxIn.hpp>
#include <includes/TxOut.hpp>

using outputs = std::vector<TxOut>;
using inputs = std::vector<TxIn>;

class Transaction
{
public:
    Transaction() = default;

    ~Transaction() = default;

    Transaction(const std::string& file_name);

    Transaction(int32_t version, const inputs& input, const outputs& output, uint32_t lock_time);

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

private:
    int32_t version_;

    byte tx_in_count_;
    inputs tx_in_;

    byte tx_out_count_;
    outputs tx_out_;

    uint32_t lock_time_;
};

#endif // TRANSACTION_HPP
