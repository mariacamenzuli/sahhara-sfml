#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"
#include "SimulationProperties.h"

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Clock.hpp>
#include <queue>

GameSimulation::GameSimulation(int gameId,
           std::unique_ptr<sf::TcpSocket> player1TcpConnection,
           std::unique_ptr<sf::TcpSocket> player2TcpConnection) : gameId(gameId),
                                                                  player1TcpConnection(std::move(player1TcpConnection)),
                                                                  player2TcpConnection(std::move(player2TcpConnection)),
                                                                  logger(ThreadLogger("game-" + std::to_string(gameId))) {
    this->player1TcpConnection->setBlocking(false);
    this->player2TcpConnection->setBlocking(false);
}

GameSimulation::~GameSimulation() {
    player1TcpConnection->disconnect();
    player2TcpConnection->disconnect();
}

void GameSimulation::run() {
    logger.info("Starting up.");

    initialize();

    // todo: send starting positions to clients on initialization
    gameState.player1Position.x = SimulationProperties::MIN_X_BOUNDARY;
    gameState.player1Position.y = SimulationProperties::MAX_Y_BOUNDARY;

    gameState.player2Position.x = SimulationProperties::MAX_X_BOUNDARY;
    gameState.player2Position.y = SimulationProperties::MAX_Y_BOUNDARY;

    sf::Clock clock;
    auto timeSinceLastUpdate = sf::Time::Zero;

    while (!gameShouldEnd) {
        timeSinceLastUpdate += clock.restart();

        checkForNetworkUpdates();

        while (timeSinceLastUpdate > timePerSimulationTick) {
            timeSinceLastUpdate -= timePerSimulationTick;

            movePlayers(timeSinceLastUpdate);
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

void GameSimulation::checkForNetworkUpdates() {
    sf::Packet player1CommandPacket;
    if (player1TcpConnection->receive(player1CommandPacket) == sf::Socket::Done) {
        sf::Int8 command;
        player1CommandPacket >> command;
        if (command == ClientSignal::MOVE_LEFT_COMMAND) {
            gameState.player1MovementQueue.push(Command::MOVE_LEFT);
        } else {
            gameState.player1MovementQueue.push(Command::MOVE_RIGHT);
        }
    }

    sf::Packet player2CommandPacket;
    if (player2TcpConnection->receive(player2CommandPacket) == sf::Socket::Done) {
        sf::Int8 command;
        player2CommandPacket >> command;
        if (command == ClientSignal::MOVE_LEFT_COMMAND) {
            gameState.player2MovementQueue.push(Command::MOVE_LEFT);
        } else {
            gameState.player2MovementQueue.push(Command::MOVE_RIGHT);
        }
    }
}

void GameSimulation::movePlayers(sf::Time deltaTime) {
    if (!gameState.player1MovementQueue.empty()) {
        Command command = gameState.player1MovementQueue.front();
        if (command == Command::MOVE_LEFT) {
            logger.info("Player 1 move left");
        } else {
            logger.info("Player 1 move right");
        }
        gameState.player1MovementQueue.pop();

        gameState.player1Position = gameState.player1Position + sf::Vector2f(SimulationProperties::RUN_VELOCITY, 0) * deltaTime.asSeconds();

        if (gameState.player1Position.x < SimulationProperties::MIN_X_BOUNDARY) {
            gameState.player1Position.x = SimulationProperties::MIN_X_BOUNDARY;
        } else if (gameState.player1Position.x > SimulationProperties::MAX_X_BOUNDARY) {
            gameState.player1Position.x = SimulationProperties::MAX_X_BOUNDARY;
        }

        if (gameState.player1Position.y < SimulationProperties::MAX_Y_BOUNDARY) {
            gameState.player1Position.y = SimulationProperties::MAX_Y_BOUNDARY;
        }
    }

    if (!gameState.player2MovementQueue.empty()) {
        Command command = gameState.player2MovementQueue.front();
        if (command == Command::MOVE_LEFT) {
            logger.info("Player 2 move left");
        } else {
            logger.info("Player 2 move right");
        }
        gameState.player2MovementQueue.pop();

        gameState.player2Position = gameState.player2Position + sf::Vector2f(SimulationProperties::RUN_VELOCITY, 0) * deltaTime.asSeconds();

        if (gameState.player2Position.x < SimulationProperties::MIN_X_BOUNDARY) {
            gameState.player2Position.x = SimulationProperties::MIN_X_BOUNDARY;
        } else if (gameState.player2Position.x > SimulationProperties::MAX_X_BOUNDARY) {
            gameState.player2Position.x = SimulationProperties::MAX_X_BOUNDARY;
        }

        if (gameState.player2Position.y > SimulationProperties::MAX_Y_BOUNDARY) {
            gameState.player2Position.y = SimulationProperties::MAX_Y_BOUNDARY;
        }
    }
}
