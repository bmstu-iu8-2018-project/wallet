#pragma once
#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <includes/http_client_async_ssl.hpp>

auto const HOST = "chain.so";
auto const PORT = "443";
auto const VERSION = 11;

const std::string BTCTEST_NETWORK = "BTCTEST/";
const std::string ROOT = "/api/v2/";
const std::string BALANCE = "get_address_balance/";
const std::string GET_TX = "get_tx/";
const std::string GET_TX_SPENT = "get_tx_spent/";
const std::string GET_TX_UNSPENT = "get_tx_unspent/";
const std::string GET_TX_RECEIVED = "get_tx_received/";
const std::string GET_TX_INPUTS = "get_tx_inputs/";
const std::string GET_TX_OUTPUTS = "get_tx_outputs/";
const std::string SEND_TX = "send_tx/";

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
