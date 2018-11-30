#pragma once
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

    Transaction(int32_t version, uint32_t lock_time, const inputs& input, const outputs& output);

    Transaction(Transaction&& other);

    Transaction(const Transaction& other);

    Transaction& operator=(Transaction&& other);

    Transaction& operator=(const Transaction& other);

    std::string get_hex_tx() const;

    std::vector<byte> get_byte_tx() const;

    void sign(const std::string& private_key_wif);

    bool is_signed() const;

    static Transaction parse(const std::string& file_name);

private:
    int32_t version_;

    uint64_t tx_in_count_;
    inputs tx_in_;

    uint64_t tx_out_count_;
    outputs tx_out_;

    uint32_t lock_time_;
};

#endif // TRANSACTION_HPP
