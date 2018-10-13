#pragma once

#include "AuthoritativeGameUpdate.h"

#include <SFML/Network/TcpSocket.hpp>
#include <memory>


class GameServerConnection {
public:
    GameServerConnection();
    ~GameServerConnection();

    enum class NonBlockingNetworkOperationStatus {
        NOT_READY,
        ERROR,
        COMPLETE
    };

    bool connectToGameLobby();
    NonBlockingNetworkOperationStatus findGame();
    NonBlockingNetworkOperationStatus acceptGame();
    NonBlockingNetworkOperationStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetworkOperationStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

private:
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;
    char tcpDataReceiveBuffer[10];

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
};
