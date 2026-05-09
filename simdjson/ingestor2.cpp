#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include "simdjson.h"


using namespace simdjson;

namespace asio = boost::asio;
namespace websocket = boost::beast::websocket;
using tcp = asio::ip::tcp;

int main() {
    try {
        std::string host = "stream.binance.com";
        std::string port = "9443";
        std::string target = "/ws/btcusdt@aggTrade";

        asio::io_context ioc;
        asio::ssl::context ctx{asio::ssl::context::tlsv12_client};
        tcp::resolver resolver{ioc};
        websocket::stream<asio::ssl::stream<tcp::socket>> ws{ioc, ctx};

        // 1. Resolve and Connect
        auto const results = resolver.resolve(host, port);
        asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // 2. SSL Handshake
        ws.next_layer().handshake(asio::ssl::stream_base::client);

        // 3. WebSocket Handshake (Crucial for Binance)
        ws.handshake(host, target);

        std::cout << "SUCCESS: Handshake complete. Watching live data..." << std::endl;

        boost::beast::flat_buffer buffer;
        long messageCount = 0;
        auto lastPrint = std::chrono::steady_clock::now();

        // simd work 
        ondemand::parser parser;

        while (true) {
            ws.read(buffer); // This blocks until a full message arrives

            messageCount++;
            
            // std::cout << "Raw Buffer: " << boost::beast::buffers_to_string(buffer.data()) << std::endl;
            // 1. Create a padded string. 
            // This copies the data, but it's the safest way to start.
            // We use the constructor that takes a pointer and a size.
            auto json_data = padded_string(static_cast<char*>(buffer.data().data()), buffer.size());

            try {
                // 2. Iterate now works because json_data is a 'padded_string'
                ondemand::document doc = parser.iterate(json_data);
                
                // 3. Extract as string_view (this is just a pointer, very fast)
                std::string_view price = doc["p"];
                std::string_view quantity = doc["q"];

                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - lastPrint).count() >= 1) {
                    std::cout << "Price: " << price << " | Qty: " << quantity << std::endl;
                    std::cout << "Messages/sec: " << messageCount << std::endl;
                    messageCount = 0;
                    lastPrint = now; // IMPORTANT: Reset lastPrint or it prints every time after 1s
                }
            } catch (simdjson_error &e) {
                // std::cerr << "Parsing error: " << e.what() << std::endl;
            }

            buffer.consume(buffer.size());

        }
    } catch (std::exception const& e) {
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
    }
    return 0;
}
