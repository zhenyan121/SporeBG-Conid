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
    // 设置等待状态
    void setShouldWait(bool shouldWait) { m_shouldWait = shouldWait; }
    void setCallbackes(MoveCallback onOpponentMove, TurnCallback onMyTurn, TurnCallback onGameStart);

    void connect(const std::string& host, int port, bool iAmFirst = true);

    void sentClickPosition(const NetData& data, bool isChangeTurn = false);

    
    

private:
    asio::io_context& m_ioContext;  // 添加引用
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_socket;
    std::string m_host;
    int m_port;
    bool m_isHost = false;
    MoveCallback m_onOpponentMove;
    TurnCallback m_onMyTurn;
    TurnCallback m_onGameStart;
    char m_readBuffer[NetData::size()];
    

     // 新增状态控制
    bool m_shouldWait = false;  // 是否应该等待对手
    bool m_isWaiting = false;    // 当前是否正在等待
    bool m_isMyTurn = false;     // 是否是我的回合

    void onConnected(bool iAmFirst);
    void waitForOpponent();
    void stopWaiting();  // 停止等待
    
    void closeConnection();
    void attemptReconnect(); // 尝试重新连接
};