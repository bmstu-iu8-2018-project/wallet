#include "includes/NetworkUtils.h"

std::string get_request_from_target(const std::string& target)
{
    boost::asio::io_context io_service;
    ssl::context ctx(ssl::context::sslv23_client);
    ctx.set_default_verify_paths();

    auto  request = std::make_shared<session>(io_service, ctx);
    request->run(host, port, target.c_str(), version);
    io_service.run();

    return request->get_body();
}

std::string post_request_from_target(
    const std::string& target,
    const std::string& body)
{
    boost::asio::io_context io_service;
    ssl::context ctx(ssl::context::sslv23_client);
    ctx.set_default_verify_paths();

    auto  request = std::make_shared<session>(io_service, ctx);
    request->run(host, port, target.c_str(), version, body);
    io_service.run();

    return request->get_body();
}

std::string get_address_balance(const std::string& address)
{
    auto const target = root + balance + btctest_network + address;
    return  get_request_from_target(target);
}

std::string get_spent_transactions(const std::string& address)
{
    auto const  target = root + get_tx_spent + btctest_network + address;
    return  get_request_from_target(target);
}

std::string get_unspent_transactions(const std::string& address)
{
    auto const target = root + get_tx_unspent + btctest_network + address;
    return  get_request_from_target(target);
}

std::string get_received_transactions(const std::string& address)
{
    auto const target = root + get_tx_received + btctest_network + address;
    return  get_request_from_target(target);
}

std::string get_transaction_inputs(const std::string& txid)
{
    auto const  target = root + get_tx_inputs + btctest_network + txid;
    return  get_request_from_target(target);
}

std::string get_transaction_outputs(const std::string& txid)
{
    auto const  target = root + get_tx_outputs + btctest_network + txid;
    return  get_request_from_target(target);
}

std::string get_transaction(const std::string& txid)
{
    auto const target = root + get_tx + btctest_network + txid;
    return  get_request_from_target(target);
}

std::string send_transaction(const std::string& tx_hex)
{
    //Takes a signed transaction in hex format and sends it to the specified network.This is a POST(!) method.
    auto const target = root + send_tx + btctest_network;
    return  post_request_from_target(target, tx_hex);
}
