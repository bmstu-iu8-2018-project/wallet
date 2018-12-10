#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <includes/http_client_async_ssl.hpp>

const auto HOST = "chain.so";
const auto POST_HOST = "api.blockcypher.com";
const auto PORT = "443";
const auto VERSION = 11;

const std::string BTCTEST_NETWORK = "BTCTEST/";
const std::string ROOT = "/api/v2/";
const std::string BALANCE = "get_address_balance/";
const std::string GET_TX = "get_tx/";
const std::string GET_TX_SPENT = "get_tx_spent/";
const std::string GET_TX_UNSPENT = "get_tx_unspent/";
const std::string GET_TX_RECEIVED = "get_tx_received/";
const std::string GET_TX_INPUTS = "get_tx_inputs/";
const std::string GET_TX_OUTPUTS = "get_tx_outputs/";
// token is a unique id in Blockcypher.com for blockchain developers
const std::string POST_TARGET =
"/v1/btc/test3/txs/push?token=5bd1d760bfa647db8db2ad11409be6ef";

namespace nu
{
    std::string get_request_from_target(const std::string& target);

    std::string post_request_from_target(
        const std::string& target,
        const std::string& body);

    std::string get_address_balance(const std::string& address);

    std::string get_spent_transactions(const std::string& address);

    std::string get_unspent_transactions(const std::string& address);

    std::string get_received_transactions(const std::string& address);

    std::string get_transaction_inputs(const std::string& txid);

    std::string get_transaction_outputs(const std::string& txid);

    std::string get_transaction(const std::string& txid);

    std::string send_transaction(const std::string& tx_hex);

} // namespace nu

#endif // NETWORK_UTILS_HPP
