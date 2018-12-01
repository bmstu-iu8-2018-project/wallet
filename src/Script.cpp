#include <includes/Script.hpp>

Script::Script(const Script& other)
    : script_(other.script_)
{}

Script::Script(const std::vector<byte>& bytes)
    : script_(bytes)
{}

Script::Script(std::vector<byte>&& bytes)
    : script_(std::move(bytes))
{}

Script& Script::operator=(Script&& other)
{
    script_ = std::move(other.script_);
    return *this;
}

void Script::create_pkh_script(const std::string& address)
{
    std::vector<byte> decode_address;
    cu::from_base58(address, decode_address);

    std::vector<byte> pkh_script =
    { OP_DUP, OP_HASH160, BYTES_TO_PUSH, OP_EQUALVERIFY, OP_CHECKSIG };

    pkh_script.insert(pkh_script.begin() + 3,
        decode_address.begin() + 1, decode_address.begin() + 21);

    script_ = pkh_script;
}

std::vector<byte> Script::data() const
{
    return script_;
}

byte Script::get_length() const
{
    return static_cast<byte>(script_.size());
}
