#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "ThreadLogger.h"

class Game {
public:
    Game(int gameId, std::unique_ptr<sf::TcpSocket> player1TcpConnection, std::unique_ptr<sf::TcpSocket> player2TcpConnection);
    ~Game();

    void run();

private:
    int gameId;
    std::unique_ptr<sf::TcpSocket> player1TcpConnection;
    std::unique_ptr<sf::TcpSocket> player2TcpConnection;
    ThreadLogger logger;

    void initialize();
};
