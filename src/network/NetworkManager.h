#pragma once

#include "network/client/Client.h"
#include "network/server/GameServer.h"

class NetworkManager {
public:
    using ClickEventCallback = std::function<void(int logicalX, int logicalY)>;
    using StartGameCallback = std::function<void()>;   
    NetworkManager();
        

    ~NetworkManager();
   

    void init(NetType type);

    void setClickEventCallback(ClickEventCallback callback);
    
    void setStartGameCallback(StartGameCallback callback) {
        m_startGameCallback = callback;
    }

    void setIsMyTurn(bool isMyTurn);
     bool isMyTurn() const { return m_isMyTurn; }
    void postClickPosition(int logicalX, int logicalY, bool isChangeTurn = false);

private:
// 一定要在最前面
    asio::io_context m_ioContext;
    //asio::ip::tcp::acceptor m_acceptor;

    std::shared_ptr<GameServer> m_gameServer = nullptr;
    std::shared_ptr<Client> m_client = nullptr;
    NetType m_netType;
    // work_guard 防止 io_context 在没有任务时退出
    asio::executor_work_guard<
        asio::io_context::executor_type> m_workguard;
    std::thread m_ioThread;
    
    ClickEventCallback m_clickEventCallback;
    StartGameCallback m_startGameCallback;

    bool m_isMyTurn = false;  // 新增：当前是否是我的回合
    void startServer();

    void startClient();

    void startIOContextLoop();
};