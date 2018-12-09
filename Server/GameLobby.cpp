#include "GameLobby.h"
#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"

#include <SFML/Network/Packet.hpp>
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

    lobbyListenerSocket.setBlocking(false);
    while (shouldRun) {
        std::unique_ptr<sf::TcpSocket> newPlayerConnection(new sf::TcpSocket());
        auto status = lobbyListenerSocket.accept(*newPlayerConnection);
        if (status == sf::Socket::NotReady) {
            for (int i = 0; i < ongoingGames.size(); i++) {
                if (ongoingGames.at(i).simulation->isGameOver()) {
                    logger.info("Game " + std::to_string(ongoingGames.at(i).simulation->getGameId()) + " has concluded");
                    ongoingGames.erase(ongoingGames.begin() + i);
                    i--;

                    if (clientsAwaitingGame.size() >=2) {
                        // todo start game
                    }
                }
            }

            continue;
        } else if (status != sf::Socket::Done) {
            logger.info("Failed to accept a new TCP connection.");
            continue;
        }

        logger.info("----------------------------------");
        logger.info("Accepted connection from [" + newPlayerConnection->getRemoteAddress().toString() + ":" + std::to_string(newPlayerConnection->getRemotePort()) + "].");

        logger.info("Sending ping packet.");
        sf::Clock pingTimer;
        sf::Packet pingPacket;
        pingPacket << "ping";
        newPlayerConnection->send(pingPacket);
        pingTimer.restart();

        logger.info("Waiting for pong packet.");
        sf::Packet pongPacket;
        newPlayerConnection->receive(pongPacket);

        auto ping = pingTimer.getElapsedTime().asMilliseconds();
        logger.info("Ping measured at " + std::to_string(ping) + "ms.");

        if (ping > 600) {
            logger.info("Ping is too high. Dropping connection.");
            sf::Packet pingTooHighPacket;
            pingTooHighPacket << static_cast<sf::Int8>(ServerSignal::PING_TOO_HIGH);
            newPlayerConnection->send(pingTooHighPacket);
            newPlayerConnection->disconnect();
            continue;
        } else {
            sf::Packet pingOkPacket;
            pingOkPacket << static_cast<sf::Int8>(ServerSignal::PING_OK);
            newPlayerConnection->send(pingOkPacket);
        }

        if (clientsAwaitingGame.empty()) {
            logger.info("Not enough players to start a game yet.");
            logger.info("Queueing new connection for match up.");
            clientsAwaitingGame.push(std::move(newPlayerConnection));
        } else if (ongoingGames.size() >= maxOngoingGames) { // todo: then we need to check if any new games can be started if an ongoing game terminates
            logger.info("Number of ongoing games is at the maximum allowed.");
            logger.info("Queueing new connection for match up.");
            clientsAwaitingGame.push(std::move(newPlayerConnection));
        } else {
            sf::TcpSocket* opponentMatched = clientsAwaitingGame.front().get();
            logger.info("Matched with queueing connection [" + opponentMatched->getRemoteAddress().toString() + ":" + std::to_string(opponentMatched->getRemotePort()) + "].");
            logger.info("Verifying connections.");

            unsigned short player1RemoteUdpPort;
            unsigned short player2RemoteUdpPort;
            const bool opponentReady = isReadyForGame(opponentMatched, player1RemoteUdpPort);
            const bool newPlayerReady = isReadyForGame(newPlayerConnection.get(), player2RemoteUdpPort);

            if (opponentReady && newPlayerReady) {
                logger.info("Starting new game thread.");

                signalGameOn(opponentMatched);
                signalGameOn(newPlayerConnection.get());

                uniqueGamesStarted++;
                std::unique_ptr<GameSimulation> gameSimulation(new GameSimulation(uniqueGamesStarted,
                                                                                  std::move(clientsAwaitingGame.front()),
                                                                                  player1RemoteUdpPort,
                                                                                  std::move(newPlayerConnection),
                                                                                  player2RemoteUdpPort));
                clientsAwaitingGame.pop();
                std::thread gameThread(&GameSimulation::run, gameSimulation.get());
                gameThread.detach();

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

bool GameLobby::isReadyForGame(sf::TcpSocket* playerConnection, unsigned short& udpPort) {
    logger.info("Sending game found signal to [" + playerConnection->getRemoteAddress().toString() + ":" + std::to_string(playerConnection->getRemotePort()) + "]");

    sf::Packet matchFoundPacket;
    matchFoundPacket << static_cast<sf::Int8>(ServerSignal::FOUND_GAME_MATCH);
    if (playerConnection->send(matchFoundPacket) != sf::Socket::Done) {
        return false;
    }

    sf::Packet matchAcceptedPacket;
    if (playerConnection->receive(matchAcceptedPacket) == sf::Socket::Done) {
        sf::Int8 matchAcceptedSignal;
        matchAcceptedPacket >> matchAcceptedSignal;

        if (matchAcceptedSignal != ClientSignal::READY_FOR_MATCH) {
            return false;
        }

        matchAcceptedPacket >> udpPort;
    } else {
        return false;
    }

    logger.info("Player ready for game.");
    return true;
}

void GameLobby::signalGameOn(sf::TcpSocket* playerConnection) {
    sf::Packet gameOnPacket;
    gameOnPacket << static_cast<sf::Int8>(ServerSignal::GAME_ON);
    playerConnection->send(gameOnPacket);
}

void GameLobby::signalGameOff(sf::TcpSocket* playerConnection) {
    sf::Packet gameOffPacket;
    gameOffPacket << static_cast<sf::Int8>(ServerSignal::GAME_OFF);
    playerConnection->send(gameOffPacket);
}
