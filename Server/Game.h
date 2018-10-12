#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

class Game {
public:
    Game(std::unique_ptr<sf::TcpSocket> player1TcpConnection, std::unique_ptr<sf::TcpSocket> player2TcpConnection);
    ~Game();

    void run();

private:
    std::unique_ptr<sf::TcpSocket> player1TcpConnection;
    std::unique_ptr<sf::TcpSocket> player2TcpConnection;

    void initialize();
};
