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

    struct GameInitInfo {
        // NonBlockingNetworkOperationStatus connectionStatus;
        bool isPlayer1;
    };

    bool connectToGameLobby();
    NonBlockingNetworkOperationStatus findGame();
    NonBlockingNetworkOperationStatus acceptGame();
    NonBlockingNetworkOperationStatus verifyGameLaunch(bool* gameOn);

private:
    const char serverFoundGameMatchSignal = '~';
    const char clientReadyForMatchSignal = '!';

    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
};
