#include "GameLobby.h"
#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <thread>

GameLobby::GameLobby(): logger(ThreadLogger("game-lobby")) {
}

GameLobby::~GameLobby() {
    logger.info("Shutting down server.");

    lobbyListenerSocket.close();
    while (!clientsAwaitingGame.empty()) {
        sf::TcpSocket* clientInQueue = clientsAwaitingGame.front().get();
        logger.info("Closing connection to [" + clientInQueue->getRemoteAddress().toString() + ":" + std::to_string(clientInQueue->getRemotePort()) + "].");
        clientInQueue->disconnect();
        clientsAwaitingGame.pop();
    }

    for (auto& ongoingGame : ongoingGames) {
        logger.info("Terminating game-" + std::to_string(ongoingGame.simulation->getGameId()));
        ongoingGame.simulation->terminate();
        ongoingGame.thread.join();
    }
}

void GameLobby::run() {
    if (lobbyListenerSocket.listen(53000) != sf::Socket::Done) {
        throw std::runtime_error("Failed to start listening on TCP socket 53000.");
    }

    logger.info("Listening for new game requests on TCP port 53000.");

    while (shouldRun) {
        lobbyListenerSocket.setBlocking(false);
        std::unique_ptr<sf::TcpSocket> newPlayerConnection(new sf::TcpSocket());
        auto status = lobbyListenerSocket.accept(*newPlayerConnection);
        if (status == sf::Socket::NotReady) {
            continue;
        } else if (status != sf::Socket::Done) {
            logger.info("Failed to accept a new TCP connection.");
            continue;
        }
        lobbyListenerSocket.setBlocking(true);

        logger.info("----------------------------------");
        logger.info("Accepted connection from [" + newPlayerConnection->getRemoteAddress().toString() + ":" + std::to_string(newPlayerConnection->getRemotePort()) + "].");

        if (clientsAwaitingGame.empty()) {
            logger.info("Not enough players to start a game yet.");
            logger.info("Queueing new connection for match up.");
            clientsAwaitingGame.push(std::move(newPlayerConnection));
        } else if (ongoingGames.size() >= maxOngoingGames) {
            logger.info("Number of ongoing games is at the maximum allowed.");
            logger.info("Queueing new connection for match up.");
            clientsAwaitingGame.push(std::move(newPlayerConnection));
        } else {
            sf::TcpSocket* opponentMatched = clientsAwaitingGame.front().get();
            logger.info("Matched with queueing connection [" + opponentMatched->getRemoteAddress().toString() + ":" + std::to_string(opponentMatched->getRemotePort()) + "].");
            logger.info("Verifying connections.");

            const bool opponentReady = isReadyForGame(opponentMatched);
            const bool newPlayerReady = isReadyForGame(newPlayerConnection.get());

            if (opponentReady && newPlayerReady) {
                logger.info("Starting new game thread.");

                signalGameOn(opponentMatched);
                signalGameOn(newPlayerConnection.get());

                uniqueGamesStarted++;
                std::unique_ptr<GameSimulation> gameSimulation(new GameSimulation(uniqueGamesStarted, std::move(clientsAwaitingGame.front()), std::move(newPlayerConnection)));
                clientsAwaitingGame.pop();
                std::thread gameThread(&GameSimulation::run, gameSimulation.get());

                ongoingGames.emplace_back(std::move(gameSimulation), gameThread);
            } else {
                if (!opponentReady) {
                    logger.info("Connection to [" + opponentMatched->getRemoteAddress().toString() + ":" + std::to_string(opponentMatched->getRemotePort()) + "] has been lost.");
                    opponentMatched->disconnect();
                    clientsAwaitingGame.pop();
                } else {
                    signalGameOff(opponentMatched);
                }

                if (!newPlayerReady) {
                    logger.info("Connection to [" + newPlayerConnection->getRemoteAddress().toString() + ":" + std::to_string(newPlayerConnection->getRemotePort()) + "] has been lost.");
                    newPlayerConnection->disconnect();
                } else {
                    signalGameOff(newPlayerConnection.get());
                    logger.info("Queueing new connection for match up.");
                    clientsAwaitingGame.push(std::move(newPlayerConnection));
                }
            }
        }
    }
}

void GameLobby::terminate() {
    shouldRun = false;
}

bool GameLobby::isReadyForGame(sf::TcpSocket* playerConnection) {
    char dataReceived[1];
    std::size_t received;

    if (playerConnection->send(&ServerSignal::FOUND_GAME_MATCH, 1) != sf::Socket::Done) {
        return false;
    }

    if (playerConnection->receive(dataReceived, 1, received) != sf::Socket::Done || dataReceived[0] != ClientSignal::READY_FOR_MATCH) {
        return false;
    }

    return true;
}

void GameLobby::signalGameOn(sf::TcpSocket* playerConnection) {
    playerConnection->send(&ServerSignal::GAME_ON, 1);
}

void GameLobby::signalGameOff(sf::TcpSocket* playerConnection) {
    playerConnection->send(&ServerSignal::GAME_OFF, 1);
}
