#pragma once

#include "AuthoritativeGameUpdate.h"

#include <SFML/Network/TcpSocket.hpp>
#include <memory>


class GameServerConnection {
public:
    GameServerConnection();
    ~GameServerConnection();

    enum class NonBlockingNetOpStatus {
        NOT_READY,
        ERROR,
        COMPLETE
    };

    bool connectToGameLobby();
    NonBlockingNetOpStatus findGame();
    NonBlockingNetOpStatus acceptGame();
    NonBlockingNetOpStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetOpStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

private:
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
};
