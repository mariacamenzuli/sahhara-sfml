#pragma once

#include "ThreadLogger.h"
#include "GameSimulation.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <queue>
#include <memory>
#include <thread>

class GameLobby {
public:
    GameLobby();
    ~GameLobby();

    void run();
    void terminate();

private:
    struct OngoingGame {
        std::unique_ptr<GameSimulation> simulation;
        std::thread thread;

        OngoingGame(std::unique_ptr<GameSimulation> gameSimulation,
                    std::thread& thread): simulation(std::move(gameSimulation)),
                                          thread(std::move(thread)) {
        }
    };

    bool shouldRun = true;
    ThreadLogger logger;
    sf::TcpListener lobbyListenerSocket;
    const int maxOngoingGames = 2;
    int uniqueGamesStarted = 0;
    std::queue<std::unique_ptr<sf::TcpSocket>> clientsAwaitingGame;
    std::vector<OngoingGame> ongoingGames;

    bool isReadyForGame(sf::TcpSocket* playerConnection, unsigned short& udpPort);
    void signalGameOn(sf::TcpSocket* playerConnection);
    void signalGameOff(sf::TcpSocket* playerConnection);
};
