#include <includes/Transaction.hpp>

Transaction::Transaction(const Transaction& other)
    : version_(other.version_)
    , lock_time_(other.lock_time_)
    , tx_in_(other.tx_in_)
    , tx_in_count_(other.tx_in_count_)
    , tx_out_(other.tx_out_)
    , tx_out_count_(other.tx_out_count_)
{};

Transaction::Transaction(Transaction&& other)
    : version_(std::move(other.version_))
    , lock_time_(std::move(other.lock_time_))
    , tx_in_(std::move(other.tx_in_))
    , tx_in_count_(std::move(other.tx_in_count_))
    , tx_out_(std::move(other.tx_out_))
    , tx_out_count_(std::move(other.tx_out_count_))
{};

Transaction::Transaction(int32_t version, uint32_t lock_time, const inputs& input, const outputs& output)
    : version_(version)
    , lock_time_(lock_time)
    , tx_in_(input)
    , tx_out_(output)
{
    tx_in_count_ = input.size();
    tx_out_count_ = output.size();
};

Transaction::Transaction(const std::string& file_name)
{
    parse(file_name);
}

Transaction& Transaction::operator=(Transaction&& other)
{
    version_ = std::move(other.version_);
    lock_time_ = std::move(other.lock_time_);
    tx_in_ = std::move(other.tx_in_);
    tx_in_count_ = std::move(other.tx_in_count_);
    tx_out_ = std::move(other.tx_out_);
    tx_out_count_ = std::move(other.tx_out_count_);

    return *this;
}

Transaction& Transaction::operator=(const Transaction& other)
{
    version_ = other.version_;
    lock_time_ = other.lock_time_;
    tx_in_ = other.tx_in_;
    tx_in_count_ = other.tx_in_count_;
    tx_out_ = other.tx_out_;
    tx_out_count_ = other.tx_out_count_;

    return *this;
}

std::string Transaction::get_hex_tx() const
{
    return cu::from_bytes_to_hex(get_byte_tx());
}

std::vector<byte> Transaction::get_byte_tx() const
{
    std::vector<byte> bytes;
    const auto version = cu::to_bytes(version_);
    bytes.insert(bytes.end(), version.begin(), version.end());

    const auto tx_in_count = cu::to_varint(static_cast<uint64_t>(tx_in_count_));
    bytes.insert(bytes.end(), tx_in_count.begin(), tx_in_count.end());
    for (const auto& i : tx_in_)
    {
        const auto input = i.get_byte_input();
        bytes.insert(bytes.end(), input.begin(), input.end());
    }

    const auto tx_out_count = cu::to_varint(static_cast<uint64_t>(tx_out_count_));
    bytes.insert(bytes.end(), tx_out_count.begin(), tx_out_count.end());
    for (const auto& o : tx_in_)
    {
        const auto output = o.get_byte_input();
        bytes.insert(bytes.end(), output.begin(), output.end());
    }

    const auto lock_time = cu::to_bytes(lock_time_);
    bytes.insert(bytes.end(), lock_time.begin(), lock_time.end());

    return bytes;
}

Transaction& Transaction::parse(const std::string& file_name)
{
    std::ifstream ifs(file_name);

    std::string data =
        static_cast<std::stringstream const&>(std::stringstream() <<
            ifs.rdbuf()).str();
    ifs.close();

    Transaction tx;
    return tx;
}
    