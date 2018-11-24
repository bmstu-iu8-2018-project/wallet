#pragma once
#ifndef HTTP_CLIENT_ASYNC_SSL_HPP
#define HTTP_CLIENT_ASYNC_SSL_HPP
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


void fail(boost::system::error_code ec, char const* what);

class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    ssl::stream<tcp::socket> stream_;
    boost::beast::flat_buffer buffer_;
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;
public:
    explicit session(boost::asio::io_context& ioc, ssl::context& ctx)
        : resolver_(ioc)
        , stream_(ioc, ctx)
    {
    }

    void run(char const* host, char const* port,
        char const* target, int version);

    void run(char const* host, char const* port,
        char const* target, int version, const std::string& body);

    void on_resolve(boost::system::error_code ec,
        tcp::resolver::results_type results);

    void on_connect(boost::system::error_code ec);

    void on_handshake(boost::system::error_code ec);

    void on_write(boost::system::error_code ec,
        std::size_t bytes_transferred);

    void on_read(boost::system::error_code ec,
        std::size_t bytes_transferred);

    void on_shutdown(boost::system::error_code ec);

    std::string get_body();
};

#endif // HTTP_CLIENT_ASYNC_SSL_HPP
