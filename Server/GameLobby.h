#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <queue>
#include <memory>

class GameLobby {
public:
    GameLobby();
    ~GameLobby();

    void run();

private:
    sf::TcpListener lobbyListenerSocket;
    const int maxOngoingGames = 1;
    int ongoingGames = 0;
    std::queue<std::unique_ptr<sf::TcpSocket>> clientsAwaitingGame;

    bool isReadyForGame(sf::TcpSocket* playerConnection);
};
