#include "NetworkManager.h"
#include "utils/ConfigLoader.h"
#include <iostream>
NetworkManager::NetworkManager()
    : m_workguard(asio::make_work_guard(m_ioContext))
{
    std::cout << "NetworkManager constructor called\n";
    m_config = ConfigLoader::load("assets/config.json");
}

NetworkManager::~NetworkManager() {
    m_ioContext.stop();
    if (m_ioThread.joinable()) {
        m_ioThread.join();
    }
}

void NetworkManager::init(NetType type) {
    m_netType = type;
    if (type == NetType::HOST) {
        m_gameServer = std::make_shared<GameServer>(m_ioContext);
        //std::cout << "try to start server\n";
        startServer();

        
    }

    m_client = std::make_shared<Client>(m_ioContext);
    startClient();
    std::cout << "client started\n";
}

void NetworkManager::setClickEventCallback(ClickEventCallback callback) {
    m_clickEventCallback = callback;
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
    m_gameServer->startServer(m_config.network.port);
    
    std::cout << "start server success\n";
    //m_ioContext.run();
    
}

void NetworkManager::startClient() {
    m_client->setCallbackes(
        [this](const NetData& click) {
            /* 处理对手棋步 */
            if (m_clickEventCallback) {
                std::cout << "Received opponent move: (" 
                          << click.clickPosition.first << ", " 
                          << click.clickPosition.second << ")\n";
                m_clickEventCallback(click.clickPosition.first, click.clickPosition.second);
            }
        },
        []() {
            /* 提示用户走棋 */
            std::cout << "NetworkManager:It's your turn now!\n";
        },
        [this]() {
            /* 游戏开始回调 */
            m_startGameCallback();
            std::cout << "Game has started!\n";
        }
    );
    if (m_netType == NetType::HOST) {
        m_client->connect("127.0.0.1", m_config.network.port, true);
    }
    if (m_netType == NetType::CLIENT) {
        m_client->connect(m_config.network.serverIP, m_config.network.port, false);  
        //m_ioContext.run();
    }
    std::cout << "start client success\n";
    startIOContextLoop();
}

void NetworkManager::startIOContextLoop() {
    
    m_ioThread = std::thread([this]() {
        try {
            size_t handlers_run = m_ioContext.run(); // 记录处理了多少个handler
            std::cout << "io_context.run() exit, handlers run: " 
                      << handlers_run << " " << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "io_context.run() exception: " << e.what() << std::endl;
        }
    });
    std::cout << "IO context loop started on thread: " 
              << m_ioThread.joinable() << std::endl;
}

void NetworkManager::postClickPosition(int logicalX, int logicalY, bool isChangeTurn) {
    if (m_client) {
        NetData data;
        data.clickPosition = {logicalX, logicalY};
        // 发送位置并告诉对手是否换回合
        m_client->sentClickPosition(data, isChangeTurn);
        std::cout << "Posted click position: ("
                  << logicalX << ", " << logicalY << ")\n";
    }
}

void NetworkManager::setIsMyTurn(bool isMyTurn) {
    m_isMyTurn = isMyTurn;
    if (m_client) {
        // 如果不是我的回合，则客户端应该等待对手
        m_client->setShouldWait(!isMyTurn);
    }
    
}