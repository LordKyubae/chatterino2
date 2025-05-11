#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include <string>
#include <mutex>

class WebSocketClient
{
public:
    explicit WebSocketClient(const std::string& channelName);

    void sendMessage(const std::string& message);
    void disconnect();
    const std::string& getChannelName() const;

private:
    ix::WebSocket socket_;
    std::string channelName_;
    std::mutex sendMutex_;
};