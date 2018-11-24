#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"
#include "SimulationProperties.h"

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Clock.hpp>
#include <queue>

GameSimulation::GameSimulation(int gameId,
                               std::unique_ptr<sf::TcpSocket> player1TcpConnection,
                               unsigned short player1RemoteUdpPort,
                               std::unique_ptr<sf::TcpSocket> player2TcpConnection,
                               unsigned short player2RemoteUdpPort) : gameId(gameId),
                                                                logger(ThreadLogger("game-" + std::to_string(gameId))),
                                                                clientConnection(player1TcpConnection->getRemoteAddress(),
                                                                                 player1RemoteUdpPort,
                                                                                 player2TcpConnection->getRemoteAddress(),
                                                                                 player2RemoteUdpPort) {
    logger.info("Initializing game-" + std::to_string(gameId) +".");
    logger.info("Player 1 remote UDP address: [IP: " + player1TcpConnection->getRemoteAddress().toString() + ", Port: " + std::to_string(player1RemoteUdpPort) + "]");
    logger.info("Player 2 remote UDP address: [IP: " + player2TcpConnection->getRemoteAddress().toString() + ", Port: " + std::to_string(player2RemoteUdpPort) + "]");

    unsigned short player1UdpSocketPort;
    unsigned short player2UdpSocketPort;
    clientConnection.initialize(player1UdpSocketPort, player2UdpSocketPort);
    logger.info("Bound local UDP sockets on port " + std::to_string(player1UdpSocketPort) + " for " + std::to_string(player2UdpSocketPort) + " for players 1 & 2.");

    logger.info("Signaling game start to player 1.");
    sf::Packet player1GameOnPacket;
    player1GameOnPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_PLAYER_1 << player1UdpSocketPort;
    player1TcpConnection->send(player1GameOnPacket);

    logger.info("Signaling game start to player 2.");
    sf::Packet player2GameOnPacket;
    player2GameOnPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_NOT_PLAYER_1 << player2UdpSocketPort;
    player2TcpConnection->send(player2GameOnPacket); //todo handle error

    player1TcpConnection->disconnect();
    player2TcpConnection->disconnect();
}

GameSimulation::~GameSimulation() = default;

void GameSimulation::run() {
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

            movePlayers(timePerSimulationTick);
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

void GameSimulation::checkForNetworkUpdates() {
    ClientUpdate player1Update;
    clientConnection.getPlayer1Update(player1Update);
    if (player1Update.type == ClientUpdate::Type::MOVE) {
        if (player1Update.move.left) {
            gameState.player1MovementQueue.push(Command::MOVE_LEFT);
        } else if (player1Update.move.right) {
            gameState.player1MovementQueue.push(Command::MOVE_RIGHT);
        }
    }

    ClientUpdate player2Update;
    clientConnection.getPlayer2Update(player2Update);
    if (player2Update.type == ClientUpdate::Type::MOVE) {
        if (player2Update.move.left) {
            gameState.player2MovementQueue.push(Command::MOVE_LEFT);
        } else if (player2Update.move.right) {
            gameState.player2MovementQueue.push(Command::MOVE_RIGHT);
        }
    }
}

void GameSimulation::movePlayers(sf::Time deltaTime) {
    if (!gameState.player1MovementQueue.empty()) {
        Command command = gameState.player1MovementQueue.front();

        sf::Vector2f velocity(0.0f, 0.0f);
        if (command == Command::MOVE_LEFT) {
            logger.info("Player 1 move left");
            velocity.x -= SimulationProperties::RUN_VELOCITY;
        } else {
            logger.info("Player 1 move right");
            velocity.x += SimulationProperties::RUN_VELOCITY;
        }
        gameState.player1MovementQueue.pop();

        gameState.player1Position = gameState.player1Position + velocity * deltaTime.asSeconds();

        if (gameState.player1Position.x < SimulationProperties::MIN_X_BOUNDARY) {
            gameState.player1Position.x = SimulationProperties::MIN_X_BOUNDARY;
        } else if (gameState.player1Position.x > SimulationProperties::MAX_X_BOUNDARY) {
            gameState.player1Position.x = SimulationProperties::MAX_X_BOUNDARY;
        }

        if (gameState.player1Position.y < SimulationProperties::MAX_Y_BOUNDARY) {
            gameState.player1Position.y = SimulationProperties::MAX_Y_BOUNDARY;
        }

        clientConnection.broadcastPlayer1Position(gameState.player1Position);
    }

    if (!gameState.player2MovementQueue.empty()) {
        Command command = gameState.player2MovementQueue.front();

        sf::Vector2f velocity(0.0f, 0.0f);
        if (command == Command::MOVE_LEFT) {
            logger.info("Player 2 move left");
            velocity.x -= SimulationProperties::RUN_VELOCITY;
        } else {
            logger.info("Player 2 move right");
            velocity.x += SimulationProperties::RUN_VELOCITY;
        }
        gameState.player2MovementQueue.pop();

        gameState.player2Position = gameState.player2Position + velocity * deltaTime.asSeconds();

        if (gameState.player2Position.x < SimulationProperties::MIN_X_BOUNDARY) {
            gameState.player2Position.x = SimulationProperties::MIN_X_BOUNDARY;
        } else if (gameState.player2Position.x > SimulationProperties::MAX_X_BOUNDARY) {
            gameState.player2Position.x = SimulationProperties::MAX_X_BOUNDARY;
        }

        if (gameState.player2Position.y > SimulationProperties::MAX_Y_BOUNDARY) {
            gameState.player2Position.y = SimulationProperties::MAX_Y_BOUNDARY;
        }

        clientConnection.broadcastPlayer2Position(gameState.player2Position);
    }
}
