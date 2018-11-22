#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H
#pragma once

#include "http_client_async_ssl.h"

auto const host = "chain.so";
auto const port = "443";
auto const version = 11;

const std::string btctest_network = "BTCTEST/";
const std::string root = "/api/v2/";
const std::string balance = "get_address_balance/";
const std::string get_tx = "get_tx/";
const std::string get_tx_spent = "get_tx_spent/";
const std::string get_tx_unspent = "get_tx_unspent/";
const std::string get_tx_received = "get_tx_received/";
const std::string get_tx_inputs = "get_tx_inputs/";
const std::string get_tx_outputs = "get_tx_outputs/";
const std::string send_tx = "send_tx/";

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

#endif
