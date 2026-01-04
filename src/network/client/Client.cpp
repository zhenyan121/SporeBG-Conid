#include "Client.h"
#include <iostream>
Client::Client(asio::io_context& ioContext):
    m_ioContext(ioContext),
    m_resolver(ioContext),
    m_socket(ioContext)
{
    // 构造函数实现
}

void Client::setCallbackes(MoveCallback onOpponentMove, TurnCallback onMyTurn, TurnCallback onGameStart) {
    m_onOpponentMove = onOpponentMove;
    m_onMyTurn = onMyTurn;
    m_onGameStart = onGameStart;
}

void Client::stopWaiting() {
    m_shouldWait = false;
    m_isWaiting = false;
}

void Client::connect(const std::string& host, int port, bool iAmFirst) {
    m_host = host;
    m_port = port;
    m_isHost = iAmFirst;
    m_shouldWait = true;  // 连接后开始等待
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
    // 重置等待状态
    m_shouldWait = true;
    m_isWaiting = false;
    // 开始等待对手消息
    if (m_shouldWait) {
        waitForOpponent();
    }

}


// 发送点击位置数据给对手

void Client::sentClickPosition(const NetData& data, bool isChangeTurn) {
    auto self = shared_from_this();

    NetData sendData = data;
    sendData.type = NetDataType::CLICK_POSITION;

    
    char buffer[NetData::size()];
    sendData.serialize(buffer);
    asio::async_write(m_socket, asio::buffer(buffer, NetData::size()),
        [this, self, isChangeTurn](const asio::error_code& ec, std::size_t /*bytesTransferred*/) {
            if (!ec) {
                // 如果需要转换回合，则开始等待对手
                if (isChangeTurn) {
                    m_isMyTurn = false;
                    m_shouldWait = true;
                    if (!m_isWaiting) {
                        waitForOpponent();
                    }
                } else {
                    m_isMyTurn = true;
                }
            } else {
                std::cerr << "send failed: " << ec.message() << std::endl;
            }
        });
}

void Client::waitForOpponent() {
    if (!m_shouldWait || m_isWaiting || !m_socket.is_open()) {
        return;
    }
    m_isWaiting = true;

    // 添加调试信息
    std::cout << "DEBUG: Starting to wait for data from " 
              << m_host << ":" << m_port 
              << ", socket open: " << m_socket.is_open() 
              << ", shouldWait: " << m_shouldWait << std::endl;

    auto self = shared_from_this();

    // 设置读取超时
    m_socket.set_option(asio::ip::tcp::no_delay(true));

    m_socket.async_read_some(
        asio::buffer(m_readBuffer, NetData::size()),
        [this, self](const asio::error_code& ec, std::size_t bytesTransferred) {
            m_isWaiting = false;  // 重置等待状态
            std::cout << "DEBUG: Read callback - Bytes received: " 
                      << bytesTransferred << ", Error: " << ec.message() << std::endl;
            if (!ec) {

                if (bytesTransferred == NetData::size()) {
                    NetData netData = NetData::deserialize(m_readBuffer);

                    // 检查消息类型
                    if (netData.type == NetDataType::GAME_START) {
                        std::cout << "Game started! First player is: " << netData.firstPlayer << std::endl;
                       
                        // 判断自己是否是先手
                        bool iAmFirst = (netData.firstPlayer == 1 && m_isHost) || 
                                        (netData.firstPlayer == 2 && !m_isHost);

                        m_isMyTurn = iAmFirst;
                        if (m_onGameStart) {
                            m_onGameStart();
                        }

                        if (m_isMyTurn && m_onMyTurn) {
                            std::cout << "It's your turn now! (You are first)\n";
                            m_onMyTurn();
                        } else if (!m_isMyTurn && m_onOpponentMove) {
                            // 如果不是先手，等待对手走棋
                            std::cout << "Waiting for opponent to move...\n";
                            // 可以在这里触发一个等待对手的回调，如果需要的话
                            if (m_shouldWait) {
                                waitForOpponent();
                            }
                        }
                    }
                    else if (netData.type == NetDataType::CLICK_POSITION) {
                        // 正常的对手移动
                        if (m_onOpponentMove) {
                            std::cout << "Received opponent move: (" 
                                      << netData.clickPosition.first << ", " 
                                      << netData.clickPosition.second << ")\n";
                            m_onOpponentMove(netData);
                        }
                        // 现在轮到我了
                        //m_isMyTurn = true;
                        
                        
                        if (m_isMyTurn) {
                            m_shouldWait = false;
                            if (m_onMyTurn) {
                                m_onMyTurn();
                            }
                        } else {
                            if (m_shouldWait && m_socket.is_open()) {
                                waitForOpponent();
                            }
                        }
                    }
                    // 重置读取缓冲区以准备下一次读取
                   
                } else {
                    std::cerr << "Incomplete data received from opponent." << std::endl;
                    if (m_shouldWait) {
                        waitForOpponent();
                    }
                }
            } else {
               // 关键修复：区分不同类型的错误
                if (ec == asio::error::eof) {
                    // 对端正常关闭连接
                    std::cout << "Connection closed by peer." << std::endl;
                    // 不要重试，关闭socket
                    closeConnection();
                } else if (ec == asio::error::operation_aborted) {
                    // 操作被取消（正常情况）
                    std::cout << "Read operation cancelled." << std::endl;
                } else {
                    // 其他错误
                    std::cerr << "Read error: " << ec.message() << std::endl;
                    // 尝试重新连接
                    if (m_shouldWait) {
                        attemptReconnect();
                    }
            }
        }
    }
    );
    
}

void Client::closeConnection() {
    asio::error_code ec;
    if (m_socket.is_open()) {
        m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        // 检查错误码
        if (ec && ec != asio::error::not_connected) {
            // 处理错误（除了"未连接"之外的错误）
        }
        m_socket.close(ec);
    }
    m_isWaiting = false;
    m_shouldWait = false;
    std::cout << "Connection closed." << std::endl;
}

void Client::attemptReconnect() {
    std::cout << "Attempting to reconnect to " << m_host << ":" << m_port << "...\n";
    closeConnection();
    // 等待一段时间后重新连接
    auto self = shared_from_this();
    asio::steady_timer timer(m_ioContext, std::chrono::seconds(3));
    timer.async_wait([this, self](const asio::error_code& /*ec*/) {
        connect(m_host, m_port, m_isHost);
    });
}
