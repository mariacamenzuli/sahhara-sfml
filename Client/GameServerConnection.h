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

    enum class Command {
        MOVE_LEFT,
        MOVE_RIGHT
    };

    bool connectToGameLobby();
    void disconnectFromGameLobby();
    NonBlockingNetOpStatus findGame();
    NonBlockingNetOpStatus acceptGame();
    NonBlockingNetOpStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetOpStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

    void sendMoveCommand(Command);

private:
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
};
