#include <includes/TxOut.hpp>

TxOut::TxOut(double value, Script&& pk_script)
{
    value_ = static_cast<int64_t>(SATOSHI_COEF * value); // convert BTC to Satoshi
    pk_script_ = std::move(pk_script);
}

TxOut::TxOut(double value, const Script& pk_script)
{
    value_ = static_cast<int64_t>(SATOSHI_COEF * value); // convert BTC to Satoshi
    pk_script_ = pk_script;
}

int64_t TxOut::get_value() const
{
    return value_;
}

std::string TxOut::get_addres() const
{
    return pk_script_.get_address();
}

Script TxOut::get_script() const
{
    return pk_script_;
}

std::string TxOut::get_hex_output() const
{
    return cu::from_bytes_to_hex(get_byte_output());
}

std::vector<byte> TxOut::get_byte_output() const
{
    std::vector<byte> bytes;

    const auto value = cu::to_bytes(get_value());
    bytes.insert(bytes.end(), value.begin(), value.end());

    const auto script_lth = cu::to_varint_byte(static_cast<uint64_t>(pk_script_.get_length()));
    bytes.insert(bytes.end(), script_lth.begin(), script_lth.end());

    const auto script = pk_script_.data();
    bytes.insert(bytes.end(), script.begin(), script.end());

    return bytes;
}

TxOut TxOut::from_data(const std::vector<byte>& bytes)
{
    uint64_t value = cu::to_type<uint64_t>(bytes.begin(), bytes.begin() + sizeof(uint64_t));
    Script pk_script = std::vector<byte>(bytes.begin() + 9, bytes.end());
    return TxOut(value / SATOSHI_COEF, pk_script);
}
