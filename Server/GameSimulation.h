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
    struct PlayerGameState {
        std::queue<ClientUpdate::MoveCommand> movementQueue;
        sf::Vector2<float> position;
        float timeInAir = 0.0f;

        bool move(sf::Time deltaTime);
    };

    const sf::Time timePerSimulationTick = sf::seconds(1.f / SimulationProperties::TICKS_PER_SECOND);

    int gameId;
    ThreadLogger logger;
    GameClientConnection clientConnection;

    bool gameShouldEnd = false;
    sf::Uint16 time = 0;
    PlayerGameState player1GameState;
    PlayerGameState player2GameState;

    void checkForNetworkUpdates();
    void movePlayers(sf::Time deltaTime);
    inline void incrementTime();
};
