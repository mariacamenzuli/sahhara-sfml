#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Clock.hpp>

GameSimulation::GameSimulation(int gameId,
           std::unique_ptr<sf::TcpSocket> player1TcpConnection,
           std::unique_ptr<sf::TcpSocket> player2TcpConnection) : gameId(gameId),
                                                                  player1TcpConnection(std::move(player1TcpConnection)),
                                                                  player2TcpConnection(std::move(player2TcpConnection)),
                                                                  logger(ThreadLogger("game-" + std::to_string(gameId))) {
}

GameSimulation::~GameSimulation() {
    player1TcpConnection->disconnect();
    player2TcpConnection->disconnect();
}

void GameSimulation::run() {
    logger.info("Starting up.");

    initialize();

    sf::Clock clock;
    auto timeSinceLastUpdate = sf::Time::Zero;

    while (!gameShouldEnd) {
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerSimulationTick) {
            timeSinceLastUpdate -= timePerSimulationTick;
        }
    }

    logger.info("Shutting down.");
}

void GameSimulation::terminate() {
    gameShouldEnd = true;
}

int GameSimulation::getGameId() const {
    return gameId;
}

void GameSimulation::initialize() {
    sf::Packet player1GameOnPacket;
    player1GameOnPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_PLAYER_1;
    player1TcpConnection->send(player1GameOnPacket);

    sf::Packet player2GameOnPacket;
    player2GameOnPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_NOT_PLAYER_1;
    player2TcpConnection->send(player2GameOnPacket); //todo handle error
}
