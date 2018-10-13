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
    int gameId;
    std::unique_ptr<sf::TcpSocket> player1TcpConnection;
    std::unique_ptr<sf::TcpSocket> player2TcpConnection;
    ThreadLogger logger;
    bool gameShouldEnd = false;

    void initialize();
};
