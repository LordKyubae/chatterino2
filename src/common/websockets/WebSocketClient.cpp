#include "WebSocketClient.hpp"
#include "singletons/Settings.hpp"

#include <iostream>

WebSocketClient::WebSocketClient(const std::string &channelName)
    : channelName_(channelName)
{
    const std::string url = chatterino::Settings::instance()
                                .webSocketAddress.getValue()
                                .toStdString() +
                            "/" + channelName;
    socket_.setUrl(url);

    socket_.setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg) {
        std::cout << std::string(magic_enum::enum_name(msg->type)) << '\n';

        if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cout << "WebSocket message error: " << msg->errorInfo.reason
                      << '\n';
        }

        if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << "[" << channelName_ << "] Received: " << msg->str
                      << '\n';
        }
    });

    socket_.start();
}

void WebSocketClient::sendMessage(const std::string &message)
{
    std::lock_guard<std::mutex> lock(sendMutex_);
    socket_.send(message);
}

void WebSocketClient::disconnect()
{
    std::lock_guard<std::mutex> lock(sendMutex_);
    socket_.stop();
}

const std::string &WebSocketClient::getChannelName() const
{
    return channelName_;
}
