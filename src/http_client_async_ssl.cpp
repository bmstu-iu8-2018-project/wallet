#include <includes/http_client_async_ssl.hpp>

void fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

void session::run(char const* host, char const* port,
    char const* target, int version)
{
    if (!SSL_set_tlsext_host_name(stream_.native_handle(), host))
    {
        boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
        std::cerr << ec.message() << "\n";
        return;
    }

    // Requere
    req_.version(version);
    req_.method(http::verb::get);
    req_.target(target);
    req_.set(http::field::host, host);
    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    resolver_.async_resolve(host, port,
        std::bind(&session::on_resolve, shared_from_this(),
            std::placeholders::_1, std::placeholders::_2));
}

void session::run(char const* host, char const* port,
    char const* target, int version, const std::string& body)
{
    if (!SSL_set_tlsext_host_name(stream_.native_handle(), host))
    {
        boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
        std::cerr << ec.message() << "\n";
        return;
    }

    // Requere
    req_.version(version);
    req_.method(http::verb::post);
    req_.target(target);
    req_.set(http::field::host, host);
    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req_.set(http::field::content_type, "application/json");
    auto data = "tx_hex=" + body + '\n';
    req_.set(http::field::body, data);

    resolver_.async_resolve(host, port,
        std::bind(&session::on_resolve, shared_from_this(),
            std::placeholders::_1, std::placeholders::_2));
}

void session::on_resolve(boost::system::error_code ec,
    tcp::resolver::results_type results)
{
    if (ec)
        return fail(ec, "resolve");

    boost::asio::async_connect(
        stream_.next_layer(),
        results.begin(),
        results.end(),
        std::bind(
            &session::on_connect,
            shared_from_this(),
            std::placeholders::_1));
}

void session::on_connect(boost::system::error_code ec)
{
    if (ec)
        return fail(ec, "connect");

    stream_.async_handshake(
        ssl::stream_base::client,
        std::bind(
            &session::on_handshake,
            shared_from_this(),
            std::placeholders::_1));
}

void session::on_handshake(boost::system::error_code ec)
{
    if (ec)
        return fail(ec, "handshake");

    http::async_write(stream_, req_,
        std::bind(
            &session::on_write,
            shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
}

void session::on_write(boost::system::error_code ec,
    std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");

    http::async_read(stream_, buffer_, res_,
        std::bind(
            &session::on_read,
            shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
}

void session::on_read(boost::system::error_code ec,
    std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "read");

    stream_.async_shutdown(
        std::bind(
            &session::on_shutdown,
            shared_from_this(),
            std::placeholders::_1));
}

void session::on_shutdown(boost::system::error_code ec)
{
    if ((ec.category() == boost::asio::error::get_ssl_category())
        && (SSL_R_PROTOCOL_IS_SHUTDOWN == ERR_GET_REASON(ec.value())))
    {
        stream_.lowest_layer().close();
    }
}

std::string session::get_body()
{
    return res_.body();
}