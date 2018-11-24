#include <includes/NetworkUtils.hpp>

std::string nu::get_request_from_target(const std::string& target)
{
    boost::asio::io_context io_service;
    ssl::context ctx(ssl::context::sslv23_client);
    ctx.set_default_verify_paths();

    auto  request = std::make_shared<session>(io_service, ctx);
    request->run(HOST, PORT, target.c_str(), VERSION);
    io_service.run();

    return request->get_body();
}

std::string nu::post_request_from_target(
    const std::string& target,
    const std::string& body)
{
    boost::asio::io_context io_service;
    ssl::context ctx(ssl::context::sslv23_client);
    ctx.set_default_verify_paths();

    auto  request = std::make_shared<session>(io_service, ctx);
    request->run(HOST, PORT, target.c_str(), VERSION, body);
    io_service.run();

    return request->get_body();
}

std::string nu::get_address_balance(const std::string& address)
{
    auto const target = ROOT + BALANCE + BTCTEST_NETWORK + address;
    return nu::get_request_from_target(target);
}

std::string nu::get_spent_transactions(const std::string& address)
{
    auto const  target = ROOT + GET_TX_SPENT + BTCTEST_NETWORK + address;
    return  nu::get_request_from_target(target);
}

std::string nu::get_unspent_transactions(const std::string& address)
{
    auto const target = ROOT + GET_TX_UNSPENT + BTCTEST_NETWORK + address;
    return nu::get_request_from_target(target);
}

std::string nu::get_received_transactions(const std::string& address)
{
    auto const target = ROOT + GET_TX_RECEIVED + BTCTEST_NETWORK + address;
    return nu::get_request_from_target(target);
}

std::string nu::get_transaction_inputs(const std::string& txid)
{
    auto const target = ROOT + GET_TX_INPUTS + BTCTEST_NETWORK + txid;
    return nu::get_request_from_target(target);
}

std::string nu::get_transaction_outputs(const std::string& txid)
{
    auto const target = ROOT + GET_TX_OUTPUTS + BTCTEST_NETWORK + txid;
    return nu::get_request_from_target(target);
}

std::string nu::get_transaction(const std::string& txid)
{
    auto const target = ROOT + GET_TX + BTCTEST_NETWORK + txid;
    return nu::get_request_from_target(target);
}

std::string nu::send_transaction(const std::string& tx_hex)
{
    //Takes a signed transaction in hex format and sends it to the specified network.This is a POST(!) method.
    auto const target = ROOT + SEND_TX + BTCTEST_NETWORK;
    return nu::post_request_from_target(target, tx_hex);
}
