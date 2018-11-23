#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "ThreadLogger.h"

class GameSimulation {
public:
    GameSimulation(int gameId, std::unique_ptr<sf::TcpSocket> player1TcpConnection, std::unique_ptr<sf::TcpSocket> player2TcpConnection);
    ~GameSimulation();

    void run();
    void terminate();
    int getGameId() const;

private:
    const sf::Time timePerSimulationTick = sf::seconds(1.f / 60.f);

    int gameId;
    std::unique_ptr<sf::TcpSocket> player1TcpConnection;
    std::unique_ptr<sf::TcpSocket> player2TcpConnection;
    ThreadLogger logger;
    bool gameShouldEnd = false;

    void initialize();
};
