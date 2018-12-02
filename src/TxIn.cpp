#include <includes/TxIn.hpp>

TxIn::TxIn(const OutPoint& point, const Script& script)
{
    previous_output_ = point;
    script_ = script;
    set_default_sequance();
}

void TxIn::set_previous_output(const std::string& prev_tx_hash, uint32_t index)
{
    previous_output_ = TxIn::OutPoint();
    previous_output_.set_hash(prev_tx_hash);
    previous_output_.set_index(index);
}

void TxIn::set_previous_output(const OutPoint& out)
{
    previous_output_ = out;
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

void TxIn::OutPoint::set_hash(const std::string& hash)
{
    hash_ = std::move(cu::from_hex_to_bytes(hash));
}

void TxIn::OutPoint::set_hash(const std::vector<byte>& hash)
{
    hash_ = hash;
}

void TxIn::OutPoint::set_hash(std::vector<byte>&& hash)
{
    hash_ = std::move(hash);
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

TxIn TxIn::from_data(const std::vector<byte>& data)
{
    std::vector<byte> prev_hash;
    prev_hash.insert(prev_hash.end(), data.begin(), data.begin() + 32);
    cu::to_littleendian_format(prev_hash);

    OutPoint out_point;
    out_point.set_hash(std::move(prev_hash));
    out_point.set_index(cu::to_type<uint32_t>(data.begin() + 32, data.begin() + 36));

    byte script_lth = data[36];

    std::vector<byte> script;
    if (script_lth == 0x19)
    {
        script.insert(script.end(), data.begin() + 37, data.begin() + 62);
    }
    else
    {
        script.insert(script.end(), data.begin() + 37,
                      data.begin() + 37 + static_cast<int>(script_lth));
    }

    return TxIn(out_point, Script(script));
}

std::vector<byte> TxIn::get_byte_input() const
{
    std::vector<byte> bytes;

    auto hash = previous_output_.hash_;
    cu::to_littleendian_format(hash);
    bytes.insert(bytes.end(), hash.begin(), hash.end());

    const auto index = cu::to_bytes(previous_output_.index_);
    bytes.insert(bytes.end(), index.begin(), index.end());

    const auto script_lth = cu::to_varint_byte(static_cast<uint64_t>(script_.get_length()));
    bytes.insert(bytes.end(), script_lth.begin(), script_lth.end());

    const auto script = script_.data();
    bytes.insert(bytes.end(), script.begin(), script.end());

    const auto sequance = cu::to_bytes(sequance_);
    bytes.insert(bytes.end(), sequance.begin(), sequance.end());

    return bytes;
}

void  TxIn::sign_by(const std::string& private_key_wif,
    const std::vector<byte>& tx)
{
    auto sig =
        cu::signature_to_der_byted(cu::sign(private_key_wif, tx));

    const auto ec_key = cu::get_ec_key_from_private(private_key_wif);
    const EC_POINT* pub = EC_KEY_get0_public_key(ec_key);
    BN_CTX* ctx = BN_CTX_new();
    const EC_GROUP* group = EC_KEY_get0_group(ec_key);
    const auto public_key = cu::from_hex_to_bytes(
        EC_POINT_point2hex(group, pub, POINT_CONVERSION_UNCOMPRESSED, ctx));

    sig.push_back((byte)public_key.size());
    sig.insert(sig.end(), public_key.begin(), public_key.end());

    script_.add_signature(sig);
}

size_t TxIn::get_script_length() const
{
    return static_cast<size_t>(script_.get_length());
}

std::string TxIn::get_prev_tx_hash() const
{
    return cu::from_bytes_to_hex(previous_output_.hash_);
}
