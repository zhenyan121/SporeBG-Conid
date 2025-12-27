#include "Client.h"
#include <iostream>
Client::Client(asio::io_context& ioContext):
    m_resolver(ioContext),
    m_socket(ioContext)
{
    // 构造函数实现
}

void Client::setCallbackes(MoveCallback onOpponentMove, TurnCallback onMyTurn) {
    m_onOpponentMove = onOpponentMove;
    m_onMyTurn = onMyTurn;
}

void Client::connect(const std::string& host, int port, bool iAmFirst) {
    m_host = host;
    m_port = port;
    //用shared_ptr保持对象存活
    auto self = shared_from_this();

    std::cout << "try to resolve: " << host << ":" << port << std::endl;
    // 屎山一样的lambda
    m_resolver.async_resolve(host, std::to_string(port),
        [this, self, iAmFirst](const asio::error_code& ec, asio::ip::tcp::resolver::results_type results) {
            if (!ec) {
                asio::async_connect(m_socket, results,
                    [this, self, iAmFirst](const asio::error_code& ec, const asio::ip::tcp::endpoint& /*endpoint*/) {
                        if (!ec) {
                            onConnected(iAmFirst);
                        } else {
                            // 处理连接错误
                            std::cerr << "connect failed: " << ec.message() << std::endl;
                        }
                    });
            } else {
                std::cerr << "resolve failed: " << ec.message() << std::endl;
            }
        });
}

void Client::onConnected(bool iAmFirst) {
    std::cout << "Connected to server " << m_host << ":" << m_port << std::endl;
    
    // 如果是先手，触发回调
    if (iAmFirst && m_onMyTurn) {
        m_onMyTurn();
    } else {
        // 等待对手动作
        waitForOpponent();
    }
}


// 发送点击位置数据给对手

void Client::sentClickPosition(const NetData& data) {
    auto self = shared_from_this();
    char buffer[NetData::size()];
    data.serialize(buffer);
    asio::async_write(m_socket, asio::buffer(buffer, NetData::size()),
        [this, self](const asio::error_code& ec, std::size_t /*bytesTransferred*/) {
            if (!ec) {
                // 发送成功，等待对手动作
                waitForOpponent();
            } else {
                std::cerr << "send failed: " << ec.message() << std::endl;
            }
        });
}

void Client::waitForOpponent() {
    auto self = shared_from_this();
    m_socket.async_read_some(
        asio::buffer(m_readBuffer, NetData::size()),
        [this, self](const asio::error_code& ec, std::size_t bytesTransferred) {
            if (!ec) {

                if (bytesTransferred == NetData::size()) {
                    NetData netData = NetData::deserialize(m_readBuffer);
                    // 触发对手移动回调
                    if (m_onOpponentMove) {
                        m_onOpponentMove(netData);
                    }
                    // 轮到我了
                    if (m_onMyTurn) {
                        m_onMyTurn();
                    }
                    // 重置读取缓冲区以准备下一次读取
                   
                } else {
                    std::cerr << "Incomplete data received from opponent." << std::endl;
                }
            } else {
                std::cerr << "read failed: " << ec.message() << std::endl;
            }
        }
    );
}

