#pragma once
#ifndef TXOUT_HPP
#define TXOUT_HPP

#include <cstdint>
#include <vector>
#include <includes/CryptoUtils.hpp>
#include <includes/Script.hpp>

const double SATOSHI_COEF = 100000000;

class TxOut
{
public:
    TxOut() = default;

    TxOut(double value, const Script& pk_script);

    TxOut(double value, Script&& pk_script);

    int64_t get_value() const;

    Script get_script() const;

    std::string get_hex_output() const;

    std::vector<byte> get_byte_output() const;

    ~TxOut() = default;

private:
    uint64_t value_;

    Script pk_script_;
};

#endif // TXOUT_HPP