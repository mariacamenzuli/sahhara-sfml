#pragma once

#include "AuthoritativeGameUpdate.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <memory>


class GameServerConnection {
public:
    GameServerConnection();
    ~GameServerConnection();

    enum class Command {
        MOVE_LEFT,
        MOVE_RIGHT,
        JUMP
    };

    bool connectToGameLobby();
    void disconnectFromGameLobby();
    NonBlockingNetOpStatus findGame();
    NonBlockingNetOpStatus acceptGame();
    NonBlockingNetOpStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetOpStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

    void sendMoveCommand(Command);

    void setServerGameRunningSocketPort(unsigned short serverGameRunningSocketPort);
    unsigned short getGameRunningSocketPort();

private:
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
    std::unique_ptr<sf::UdpSocket> gameRunningUdpSocket;
    unsigned short gameRunningSocketPort = 0;

    const sf::IpAddress serverIp = sf::IpAddress("127.0.0.1");
    const unsigned short serverLobbyPort = 53000;

    bool bindGameRunningConnection(unsigned short& udpSocketPort);
};
