#pragma once
#ifndef TXIN_HPP
#define TXIN_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <includes/Script.hpp>

const uint32_t DEFAULT_SEQUANCE = 0xFFFFFFFF;

class TxIn
{
public:
    TxIn() = default;
    
    void set_previous_output(const std::string& prev_tx_hash, uint32_t index);

    void set_default_sequance();

    void set_pkh_script(Script&& pkh_script);

    void set_pkh_script(const Script& pkh_script);

    uint32_t get_index() const;

    Script get_script() const;

    uint32_t get_sequance() const;

    std::string get_hex_input() const;

    std::vector<byte> get_byte_input() const;

    ~TxIn();
private:

    struct OutPoint
    {
        std::vector<byte> hash_; // The hash of the reference transaction.
        uint32_t index_; // The index of the specific output in the tx. First output is 0, etc.

        void set_hash(const std::string& hash);
        void set_index(uint32_t index);
    }; 

    OutPoint previous_output_; // The previous output transaction reference, as an OutPoint structure.
    Script script_;
    uint32_t sequance_;
};

#endif // TXIN_HPP



