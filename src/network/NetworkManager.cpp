#include "NetworkManager.h"
#include <iostream>


NetworkManager::~NetworkManager() {
    
}

void NetworkManager::init(NetType type) {
    m_netType = type;
    if (type == NetType::HOST) {
        m_gameServer = std::make_shared<GameServer>(m_ioContext);
        //std::cout << "try to start server\n";
        startServer();
        
    }
    m_client = std::make_shared<Client>(m_ioContext);
}

/*
void NetworkManager::init(NetType type) {
        // 先启动 io_context 线程
        m_ioThread = std::thread([this]() {
            std::cout << "Network thread starting: " 
                      << std::this_thread::get_id() << std::endl;
            m_ioContext.run();
        });
        std::cout << "Main thread after starting network thread: " 
                  << std::this_thread::get_id() << std::endl;
        // 等待网络线程启动
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Initializing NetworkManager on thread: " 
                  << std::this_thread::get_id() << std::endl;
        // 在网络线程中初始化
        asio::post(m_ioContext, [this, type]() {
            std::cout << "NetworkManager init running on thread: " 
                      << std::this_thread::get_id() << std::endl;
            // 在这里创建 GameServer 和调用 async_accept
            if (type == NetType::HOST) {
                m_gameServer = std::make_shared<GameServer>(m_ioContext);
                m_gameServer->startServer(52025);
            }
        });
    }*/
void NetworkManager::startServer() {
    
    if (!m_gameServer) {
        std::cerr << "gameServer is not esist\n";
    }
    m_gameServer->startServer(52025);
    
    std::cout << "start server success\n";
    m_ioContext.run();
    
}

void NetworkManager::startClient() {
    m_client->setCallbackes(
        [](const NetData& click) {
            /* 处理对手棋步 */
        },
        []() {
            /* 提示用户走棋 */
            std::cout << "It's your turn now!\n";
        }
    );
    if (m_netType == NetType::HOST) {
        m_client->connect("127.0.0.1", 52025, true);
    }
    if (m_netType == NetType::CLIENT) {
        m_client->connect("127.0.0.1", 52025, false);  
        m_ioContext.run();
    }
}