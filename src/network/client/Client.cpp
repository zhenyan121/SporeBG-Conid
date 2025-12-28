#include "Client.h"
#include <iostream>
Client::Client(asio::io_context& ioContext):
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
    if (!m_shouldWait || m_isWaiting) {
        return;
    }
    auto self = shared_from_this();
    m_socket.async_read_some(
        asio::buffer(m_readBuffer, NetData::size()),
        [this, self](const asio::error_code& ec, std::size_t bytesTransferred) {
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
                            if (m_shouldWait) {
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
                std::cerr << "read failed: " << ec.message() << std::endl;
                // 发生错误时，可以选择重新等待对手
                if (m_shouldWait) {
                    waitForOpponent();
                }
            }
        }
    );
}

