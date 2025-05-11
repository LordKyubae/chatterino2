#pragma once

#include "common/websockets/WebSocketClient.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class WebSocketManager
{
public:
    static WebSocketManager& instance();

    void connectToChannel(const std::string& channelName);
    void disconnectFromChannel(const std::string& channelName);
    void disconnectAll();
    void sendMessageToChannel(const std::string& channelName, const std::string& message);

private:
    WebSocketManager() = default;

    std::unordered_map<std::string, std::shared_ptr<WebSocketClient>> clients_;
    std::mutex mutex_;
};