#pragma once

#include "network/client/Client.h"
#include "network/server/GameServer.h"

class NetworkManager {
public:
    NetworkManager();
        

    ~NetworkManager();
   

    void init(NetType type);

    

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
    
    
    void startServer();

    void startClient();

    void startIOContextLoop();
};