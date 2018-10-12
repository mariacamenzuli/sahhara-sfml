#pragma once

#include "ThreadLogger.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <queue>
#include <memory>

class GameLobby {
public:
    explicit GameLobby(ThreadLogger& threadLogger);
    ~GameLobby();

    void run();

private:
    ThreadLogger& logger;
    sf::TcpListener lobbyListenerSocket;
    const int maxOngoingGames = 2;
    int ongoingGames = 0;
    int uniqueGamesStarted = 0;
    std::queue<std::unique_ptr<sf::TcpSocket>> clientsAwaitingGame;

    bool isReadyForGame(sf::TcpSocket* playerConnection);
    void signalGameOn(sf::TcpSocket* playerConnection);
    void signalGameOff(sf::TcpSocket* playerConnection);
};
