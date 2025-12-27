#pragma once

#include "network/client/Client.h"
#include "network/server/GameServer.h"

class NetworkManager {
public:
    NetworkManager() = default;
        

    ~NetworkManager();
   

    void init(NetType type);

    

private:
    
    //asio::ip::tcp::acceptor m_acceptor;

    std::shared_ptr<GameServer> m_gameServer = nullptr;
    std::shared_ptr<Client> m_client = nullptr;
    NetType m_netType;

    std::thread m_ioThread;
    
    asio::io_context m_ioContext;
    void startServer();

    void startClient();
};