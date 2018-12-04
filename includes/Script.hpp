#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <includes/CryptoUtils.hpp>

const byte OP_DUP = 0x76;
const byte OP_HASH160 = 0xa9;
const byte BYTES_TO_PUSH = 0x14;
const byte OP_EQUALVERIFY = 0x88;
const byte OP_CHECKSIG = 0xac;

class Script
{
public:
    Script() = default;

    ~Script() = default;

    Script(Script&& other) = default;

    Script(const Script& other);

    Script(const std::vector<byte>& bytes);

    Script(std::vector<byte>&& bytes);

    Script& operator=(Script&& other);

    Script& operator=(const Script& other) = default;

    void create_pkh_script(const std::string& address);

    std::vector<byte> data() const;

    byte get_length() const;

    void add_signature(std::vector<byte>&& sign);

private:
    std::vector<byte> script_;
};

#endif // SCRIPT_HPP
