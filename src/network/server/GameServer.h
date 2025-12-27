#pragma once

#include <asio.hpp>
#include "network/NetData.h"



class GameServer : public std::enable_shared_from_this<GameServer> {
public:
    GameServer(asio::io_context& ioContext);
    ~GameServer() = default;

    void startServer( int port);

private:
    asio::io_context& m_ioContext;
    //std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    asio::ip::tcp::acceptor  m_acceptor;
    asio::ip::tcp::socket m_player1;
    asio::ip::tcp::socket m_player2;
    char m_buffer[NetData::size()];

    void waitForPlayers(int playerNumber);
    
    void startGame();

    void forwardMoves();

    void listenPlayer(asio::ip::tcp::socket& fromPlayer, asio::ip::tcp::socket& toPlayer);
};