#include <includes/Transaction.hpp>

Transaction::Transaction(const Transaction& other)
    : version_(other.version_),
      locktime_(other.locktime_),
      tx_in_(other.tx_in_),
      tx_in_count_(other.tx_in_count_),
      tx_out_(other.tx_out_),
      tx_out_count_(other.tx_out_count_){};

Transaction::Transaction(Transaction&& other)
    : version_(std::move(other.version_)),
      locktime_(std::move(other.locktime_)),
      tx_in_(std::move(other.tx_in_)),
      tx_in_count_(std::move(other.tx_in_count_)),
      tx_out_(std::move(other.tx_out_)),
      tx_out_count_(std::move(other.tx_out_count_)){};

Transaction::Transaction(int32_t version,
                         const inputs& input,
                         const outputs& output,
                         uint32_t lock_time)
    : version_(version), locktime_(lock_time), tx_in_(input), tx_out_(output) {
  tx_in_count_ = cu::to_varint(input.size());
  tx_out_count_ = cu::to_varint(output.size());
};

Transaction::Transaction(const std::string& file_name) {
  parse(file_name);
}

Transaction& Transaction::operator=(Transaction&& other) {
  if (&other != this) {
    version_ = std::move(other.version_);
    locktime_ = std::move(other.locktime_);
    tx_in_ = std::move(other.tx_in_);
    tx_in_count_ = std::move(other.tx_in_count_);
    tx_out_ = std::move(other.tx_out_);
    tx_out_count_ = std::move(other.tx_out_count_);
  }
  return *this;
}

Transaction& Transaction::operator=(const Transaction& other) {
  if (&other != this) {
    version_ = other.version_;
    locktime_ = other.locktime_;
    tx_in_ = other.tx_in_;
    tx_in_count_ = other.tx_in_count_;
    tx_out_ = other.tx_out_;
    tx_out_count_ = other.tx_out_count_;
  }
  return *this;
}

std::string Transaction::get_hex_tx() const {
  return cu::from_bytes_to_hex(get_byte_tx());
}

std::vector<byte> Transaction::get_byte_tx() const {
  std::vector<byte> bytes;
  const auto version = cu::to_bytes(version_);
  bytes.insert(bytes.end(), version.begin(), version.end());

  const auto tx_in_count =
      cu::to_varint_byte(static_cast<uint64_t>(tx_in_count_));
  bytes.insert(bytes.end(), tx_in_count.begin(), tx_in_count.end());
  for (const auto& i : tx_in_) {
    const auto input = i.get_byte_input();
    bytes.insert(bytes.end(), input.begin(), input.end());
  }

  const auto tx_out_count = cu::to_varint_byte(tx_out_count_);
  bytes.insert(bytes.end(), tx_out_count.begin(), tx_out_count.end());
  for (const auto& o : tx_out_) {
    const auto output = o.get_byte_output();
    bytes.insert(bytes.end(), output.begin(), output.end());
  }

  const auto lock_time = cu::to_bytes(locktime_);
  bytes.insert(bytes.end(), lock_time.begin(), lock_time.end());

  return bytes;
}

Transaction Transaction::parse(const std::string& file_name) {
  std::ifstream ifs(file_name);
  std::string data;
  std::getline(ifs, data);
  ifs.close();

  const auto bytes = cu::from_hex_to_bytes(data);

  int32_t version =
      cu::to_type<int32_t>(bytes.begin(), bytes.begin() + sizeof(int32_t));

  const auto iter_in_count = bytes.begin() + sizeof(int32_t);

  uint64_t tx_in_count = cu::to_varint(static_cast<size_t>(*iter_in_count));

  inputs tx_in;
  auto iter_first_in = iter_in_count + 1;
  for (size_t i = 0; i < tx_in_count; ++i) {
    auto last = std::search_n(iter_first_in, bytes.end(), SEQUANCE_SIZE, 0xFF);
    if (last != bytes.end()) {
      std::vector<byte> vec_in(iter_first_in, last + SEQUANCE_SIZE);
      tx_in.push_back(TxIn::from_data(vec_in));
      iter_first_in = last + SEQUANCE_SIZE;
    }
  }

  uint64_t tx_out_count = cu::to_varint(static_cast<size_t>(*(iter_first_in)));

  auto iter_first_out = iter_first_in + 1;
  outputs tx_out;
  for (size_t i = 0; i < tx_out_count; ++i) {
    std::vector<byte> vec_out(iter_first_out, iter_first_out + LENGTH_OUTPUT);
    tx_out.push_back(TxOut::from_data(vec_out));
    iter_first_out += LENGTH_OUTPUT;
  }

  uint32_t lock_time = cu::to_type<int32_t>(iter_first_out, bytes.end());

  return Transaction(version, tx_in, tx_out, lock_time);
}

void Transaction::sign(const std::string& private_key_wif) {
  if (tx_in_.size() == 0)
    return;
  for (auto& input : tx_in_) {
    // every input must sign hash which include previous signed input
    auto raw_tx = get_byte_tx();
    const std::vector<byte> hash_code = {0x01, 0x00, 0x00, 0x00};
    raw_tx.insert(raw_tx.end(), hash_code.begin(), hash_code.end());

    input.sign_by(private_key_wif, raw_tx);
  }
}

bool Transaction::is_signed() const {
  if (tx_in_.size() == 0)
    return false;
  return tx_in_[0].get_script_length() > 25;
}

std::vector<TxIn> Transaction::get_inputs() const {
  return tx_in_;
}

std::vector<TxOut> Transaction::get_outputs() const {
  return tx_out_;
}

int32_t Transaction::get_version() const {
  return version_;
}

uint32_t Transaction::get_locktime() const {
  return locktime_;
}

byte Transaction::get_in_count() const {
  return tx_in_count_;
}

byte Transaction::get_out_count() const {
  return tx_out_count_;
}
