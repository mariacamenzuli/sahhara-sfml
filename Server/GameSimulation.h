#pragma once

#include "ThreadLogger.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <queue>
#include "GameClientConnection.h"

class GameSimulation {
public:
    GameSimulation(int gameId, std::unique_ptr<sf::TcpSocket> player1TcpConnection, unsigned short player1RemoteUdpPort, std::unique_ptr<sf::TcpSocket> player2TcpConnection, unsigned short player2RemoteUdpPort);
    ~GameSimulation();

    void run();
    void terminate();
    int getGameId() const;

private:
    enum class Command {
        MOVE_LEFT,
        MOVE_RIGHT,
        JUMP
    };

    struct GameState {
        std::queue<Command> player1MovementQueue;
        std::queue<Command> player2MovementQueue;
        sf::Vector2<float> player1Position;
        sf::Vector2<float> player2Position;
    };

    const sf::Time timePerSimulationTick = sf::seconds(1.f / 60.f);

    int gameId;
    ThreadLogger logger;
    GameClientConnection clientConnection;

    bool gameShouldEnd = false;
    GameState gameState;

    void checkForNetworkUpdates();
    void movePlayers(sf::Time deltaTime);
};
