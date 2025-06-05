#include "WebSocketManager.hpp"
#include <iostream>

WebSocketManager& WebSocketManager::instance()
{
    static WebSocketManager instance;
    return instance;
}

void WebSocketManager::connectToChannel(const std::string& channelName)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (clients_.find(channelName) != clients_.end())
    {
        std::cout << "Already connected to channel: " << channelName << '\n';
        return;
    }

    auto client = std::make_shared<WebSocketClient>(channelName);
    clients_[channelName] = client;
}

void WebSocketManager::disconnectFromChannel(const std::string& channelName)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = clients_.find(channelName);
    if (it != clients_.end())
    {
        it->second->disconnect();
        clients_.erase(it);
        std::cout << "Disconnected from channel: " << channelName << '\n';
    }
}

void WebSocketManager::disconnectAll()
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [_, client] : clients_)
    {
        client->disconnect();
    }
    clients_.clear();
}

void WebSocketManager::sendMessageToChannel(const std::string& channelName, bool isBroadcaster, const std::string& message)
{
    if (!isBroadcaster)
    {
        std::cout << "[" << channelName << "] Tired to send message to OBS when User is not the Broadcaster." << '\n';
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    auto it = clients_.find(channelName);
    if (it != clients_.end())
    {
        it->second->sendMessage(message);
    }
    else
    {
        std::cerr << "No connection found for channel: " << channelName << '\n';
    }
}