#pragma once

#include "ThreadLogger.h"
#include "GameClientConnection.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <queue>
#include "SimulationProperties.h"

class GameSimulation {
public:
    GameSimulation(int gameId, std::unique_ptr<sf::TcpSocket> player1TcpConnection, unsigned short player1RemoteUdpPort, std::unique_ptr<sf::TcpSocket> player2TcpConnection, unsigned short player2RemoteUdpPort);
    ~GameSimulation();

    void run();
    void terminate();
    int getGameId() const;

private:
    struct GameState {
        std::queue<ClientUpdate::MoveCommand> player1MovementQueue;
        std::queue<ClientUpdate::MoveCommand> player2MovementQueue;
        sf::Vector2<float> player1Position;
        sf::Vector2<float> player2Position;
    };

    const sf::Time timePerSimulationTick = sf::seconds(1.f / SimulationProperties::TICKS_PER_SECOND);

    int gameId;
    ThreadLogger logger;
    GameClientConnection clientConnection;

    bool gameShouldEnd = false;
    GameState gameState;

    void checkForNetworkUpdates();
    void movePlayers(sf::Time deltaTime);
};
