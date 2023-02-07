#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <boost/asio.hpp>

#include <database/context.hh>

using namespace std::literals::string_view_literals;

using ::boost::asio::ip::tcp;

class session
  : public ::std::enable_shared_from_this<session>
{
public:
    static
    constexpr
    ::std::size_t MAX_DATA_LENGTH{1024};

    [[nodiscard]]
    explicit
    session(tcp::socket socket, ::db::context& db_ctx) :
        socket_{::std::move(socket)},
        db_ctx_{db_ctx} {}

    void
    start()
    {
        this->do_work();
    }

private:
    void
    do_work()
    {
        auto self{this->shared_from_this()};

        socket_.async_read_some(
            ::boost::asio::buffer(data_, MAX_DATA_LENGTH),
            [this, self](::boost::system::error_code read_err, ::std::size_t length) {
                if (read_err) {
                    throw ::std::runtime_error{"err"};
                }

                auto const cmp_msg = "parking_space/get"sv;
                if(::std::strncmp(data_.data(), cmp_msg.data(), cmp_msg.length()) == 0) {
                    auto json = this->db_ctx_.prepare_statement("SELECT * FROM parking_space;").exec_json();
                    auto str = json.dump();
                    std::cout << str << '\n';
                    ::boost::asio::async_write(
                            socket_,
                            ::boost::asio::buffer(str.c_str(), str.length()*sizeof(::std::decay_t<decltype(str)>::value_type)),
                            [](::boost::system::error_code write_err, [[maybe_unused]] ::std::size_t) {
                                if(write_err) {
                                    throw ::std::runtime_error{"errrr"};
                                }
                            }
                    );
                } else {
                    this->do_write(length);
                }
            }
        );
    }

    void do_write(::std::size_t length)
    {
        auto self{this->shared_from_this()};

        ::boost::asio::async_write(
            socket_,
            ::boost::asio::buffer(data_, length),
            []([[maybe_unused]] ::boost::system::error_code, [[maybe_unused]] ::std::size_t) {}
            //[this, self](::boost::system::error_code err_c, [[maybe_unused]] ::std::size_t) {
            //    if (!err_c) {
            //        this->do_read();
            //    }
            //}
        );
    }

    tcp::socket socket_;
    ::std::array<char, MAX_DATA_LENGTH> data_{};
    ::db::context& db_ctx_;
};

class server
{
public:
    [[nodiscard]]
    server(::boost::asio::io_context& io_context, ::std::uint16_t port, ::db::context&& db_ctx) :
        acceptor_{io_context, tcp::endpoint{tcp::v4(), port}},
        db_ctx_{std::move(db_ctx)}
    {
        this->do_accept();
    }

private:
    void
    do_accept()
    {
        acceptor_.async_accept(
            [this](::boost::system::error_code err_c, tcp::socket socket) {
            if (!err_c) {
                ::std::make_shared<::session>(::std::move(socket), std::ref(db_ctx_))->start();
            }

            this->do_accept();
        });
    }

    tcp::acceptor acceptor_;
    ::db::context db_ctx_;
};

static
constexpr
::std::uint16_t PORT{55555};

int
main([[maybe_unused]] int argc, char** argv)
{
    try
    {
        ::db::context db_ctx{argv[1]};

        ::boost::asio::io_context io_context;
        ::server server(io_context, PORT, std::move(db_ctx));
        io_context.run();
    }
    catch (::std::exception& e)
    {
        ::std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}
