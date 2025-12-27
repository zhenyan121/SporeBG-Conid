#pragma once

#include <asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include "network/NetData.h"

class Client : public std::enable_shared_from_this<Client> {
public:

    // 事件回调类型
    using MoveCallback = std::function<void(const NetData&)>;
    using TurnCallback = std::function<void()>;

    Client(asio::io_context& ioContext);
    ~Client() = default;
    
    void setCallbackes(MoveCallback onOpponentMove, TurnCallback onMyTurn);

    void connect(const std::string& host, int port, bool iAmFirst = true);

    void sentClickPosition(const NetData& data);

    
    

private:
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_socket;
    std::string m_host;
    int m_port;
    MoveCallback m_onOpponentMove;
    TurnCallback m_onMyTurn;
    char m_readBuffer[NetData::size()];
    

    void onConnected(bool iAmFirst);
    void waitForOpponent();
    
    
};