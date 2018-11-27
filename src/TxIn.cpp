#include "TxIn.hpp"

void TxIn::set_previous_output(const std::string& prev_tx_hash, uint32_t index)
{
    previous_output_ = TxIn::OutPoint();
    previous_output_.set_hash(prev_tx_hash);
    previous_output_.set_index(index);
}

void TxIn::set_default_sequance()
{
    sequance_ = DEFAULT_SEQUANCE;
}

void TxIn::set_pkh_script(Script&& pkh_script)
{
    script_ = std::move(pkh_script);
}

void TxIn::set_pkh_script(const Script& pkh_script)
{
    script_ = pkh_script;
}

TxIn::~TxIn()
{}

void TxIn::OutPoint::set_hash(const std::string& hash)
{
    hash_ = std::move(cu::from_hex_to_bytes(hash));
}

void TxIn::OutPoint::set_index(uint32_t index)
{
    index_ = index;
}

uint32_t TxIn::get_index() const
{
    return previous_output_.index_;
}

Script TxIn::get_script() const
{
    return script_;
}

uint32_t TxIn::get_sequance() const
{
    return sequance_;
}

std::string TxIn::get_hex_input() const
{
    return cu::from_bytes_to_hex(get_byte_input());
}

std::vector<byte> TxIn::get_byte_input() const
{
    std::vector<byte> bytes;

    auto hash = previous_output_.hash_;
    cu::to_littleendian_format(hash);
    bytes.insert(bytes.end(), hash.begin(), hash.end());

    const auto index = cu::to_bytes(previous_output_.index_);
    bytes.insert(bytes.end(), index.begin(), index.end());

    const auto script_lth = cu::to_varint(static_cast<uint64_t>(script_.get_length()));
    bytes.insert(bytes.end(), script_lth.begin(), script_lth.end());

    const auto script = script_.data();
    bytes.insert(bytes.end(), script.begin(), script.end());

    const auto sequance = cu::to_bytes(sequance_);
    bytes.insert(bytes.end(), sequance.begin(), sequance.end());

    return bytes;
}


