#pragma once

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
    bool getIsPlayer1(); //todo: change into a more generic getGameUpdate that returns a union?

private:
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
};
