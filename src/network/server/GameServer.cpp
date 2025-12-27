#include "GameServer.h"
#include <iostream>
//#include <sstream>
GameServer::GameServer(asio::io_context& ioContext)
    : m_ioContext(ioContext),
    m_player1(ioContext),
    m_player2(ioContext),
    m_acceptor(ioContext)
{
        
}

void GameServer::startServer(int port) {
    
    std::cout << "Starting server on port " << port << std::endl;
    //std::cout << "startServer called on thread: " 
    //              << std::this_thread::get_id() << std::endl;
    // 同步初始化（通常这部分是同步的）
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();
        
    // 开始异步接受连接
    waitForPlayers(1);

    
    
}

void GameServer::waitForPlayers(int playerNum) {

    /*if (!m_acceptor) {
        std::cerr << "ERROR: m_acceptor is null in waitForPlayers!" << std::endl;
        return;
    }*/
    
    std::cout << "waitForPlayers called for player " << playerNum << std::endl;

    //auto& socket = (playerNum == 1) ? m_player1 : m_player2;
    std::cout << "accepting player " << playerNum << std::endl;
    // 捕获 shared_ptr 确保对象存活
    //auto self = shared_from_this();
    try {
        std::cout << "Before async_accept for player " << playerNum << std::endl;
        
        /*if (!m_acceptor || !m_acceptor->is_open()) {
            std::cerr << "Acceptor is not open!" << std::endl;
            return;
        }*/
        // 保存当前对象的原始指针（用于调试）
        /*static std::atomic<void*> lastThisPtr = nullptr;
        lastThisPtr = this;
        std::cout << "waitForPlayers: this=" << this 
                << ", playerNum=" << playerNum << std::endl;*/
        //std::cout << "acceptor open: " << m_acceptor->is_open() << "\n";
        //std::cout << "socket open: " << socket.is_open() << "\n";
        // mingw老版本有重大问题，导致asio内部状态损坏，引起段错误！！！
        m_acceptor.async_accept(
            [this, playerNum](const asio::error_code& ec, asio::ip::tcp::socket peerSocket) {
                if (!ec) {
                    std::cout << "player" << playerNum << " connect successfully\n";
                    if (playerNum == 1) {
                        m_player1 = std::move(peerSocket);
                        std::cout << "try to accept player2\n";
                        waitForPlayers(2);
                    } else {
                        m_player2 = std::move(peerSocket);
                        startGame();
                    }
                } else {
                    std::cerr << "Error accepting player " << playerNum << ": " << ec.message() << std::endl;
                }
            }
        );
        std::cout << "After async_accept for player " << playerNum << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in waitForPlayers: " << e.what() << std::endl;
    }
}

void GameServer::startGame() {
    std::cout << "Gmae Start player1 is the first\n";

    forwardMoves();
}

void GameServer::forwardMoves() {
    listenPlayer(m_player1, m_player2);
    listenPlayer(m_player2, m_player1);
}

void GameServer::listenPlayer(asio::ip::tcp::socket& fromPlayer, asio::ip::tcp::socket& toPlayer) {
    auto self = shared_from_this();

    fromPlayer.async_read_some(
        asio::buffer(m_buffer, NetData::size()),
        [this, self, &fromPlayer, &toPlayer](const asio::error_code& ec, size_t bytes) {
            if (!ec) {
                if (bytes == NetData::size()) {
                    // 转发给对手
                    asio::async_write(
                        toPlayer,
                        asio::buffer(m_buffer, bytes),
                        [](const asio::error_code& ec, size_t) {

                        }
                        
                    );
                    listenPlayer(fromPlayer, toPlayer);
                }
            }
        }    
    );
}