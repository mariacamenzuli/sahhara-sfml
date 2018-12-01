#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"
#include "SimulationProperties.h"

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Clock.hpp>
#include <queue>
#include <iostream>

GameSimulation::GameSimulation(int gameId,
                               std::unique_ptr<sf::TcpSocket> player1TcpConnection,
                               unsigned short player1RemoteUdpPort,
                               std::unique_ptr<sf::TcpSocket> player2TcpConnection,
                               unsigned short player2RemoteUdpPort) : gameId(gameId),
                                                                      logger(ThreadLogger("game-" + std::to_string(gameId))),
                                                                      clientConnection(logger,
                                                                                       player1TcpConnection->getRemoteAddress(),
                                                                                       player1RemoteUdpPort,
                                                                                       player2TcpConnection->getRemoteAddress(),
                                                                                       player2RemoteUdpPort) {
    logger.info("Initializing game-" + std::to_string(gameId) + ".");
    logger.info("Player 1 remote UDP address: [IP: " + player1TcpConnection->getRemoteAddress().toString() + ", Port: " + std::to_string(player1RemoteUdpPort) + "]");
    logger.info("Player 2 remote UDP address: [IP: " + player2TcpConnection->getRemoteAddress().toString() + ", Port: " + std::to_string(player2RemoteUdpPort) + "]");

    unsigned short player1UdpSocketPort;
    unsigned short player2UdpSocketPort;
    clientConnection.initialize(player1UdpSocketPort, player2UdpSocketPort);
    logger.info("Bound local UDP sockets on port " + std::to_string(player1UdpSocketPort) + " for " + std::to_string(player2UdpSocketPort) + " for players 1 & 2.");

    logger.info("Signaling game start to player 1.");
    sf::Packet player1GameInitPacket;
    player1GameInitPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_PLAYER_1 << player1UdpSocketPort;
    player1TcpConnection->send(player1GameInitPacket);

    logger.info("Signaling game start to player 2.");
    sf::Packet player2GameInitPacket;
    player2GameInitPacket << static_cast<sf::Int8>(ServerSignal::GAME_INIT) << ServerSignal::IS_NOT_PLAYER_1 << player2UdpSocketPort;
    player2TcpConnection->send(player2GameInitPacket); //todo handle error

    player1TcpConnection->disconnect();
    player2TcpConnection->disconnect();
}

GameSimulation::~GameSimulation() = default;

void GameSimulation::run() {
    player1GameState.position.x = SimulationProperties::MIN_X_BOUNDARY;
    player1GameState.position.y = SimulationProperties::MAX_Y_BOUNDARY;
    player1GameState.direction = SimulationProperties::Direction::RIGHT;
    player1GameState.isPlayer1 = true;

    player2GameState.position.x = SimulationProperties::MAX_X_BOUNDARY;
    player2GameState.position.y = SimulationProperties::MAX_Y_BOUNDARY;
    player2GameState.direction = SimulationProperties::Direction::LEFT;
    player2GameState.isPlayer1 = false;

    sf::Clock clock;
    auto timeSinceLastUpdate = sf::Time::Zero;

    while (!gameShouldEnd) {
        timeSinceLastUpdate += clock.restart();

        checkForNetworkUpdates();

        while (timeSinceLastUpdate > timePerSimulationTick) {
            timeSinceLastUpdate -= timePerSimulationTick;

            movePlayers(timePerSimulationTick);

            clientConnection.sendUnackedProjectileUpdates();

            incrementTime();
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

bool GameSimulation::movePlayer(PlayerGameState& playerGameState, class sf::Time deltaTime) {
    ClientUpdate::MoveCommand command;
    if (!playerGameState.movementQueue.empty()) {
        command = playerGameState.movementQueue.front();
        playerGameState.movementQueue.pop();
    } else {
        command = ClientUpdate::MoveCommand();
    }

    sf::Vector2f velocity(0.0f, 0.0f);

    if (playerGameState.attacking) {
        auto attackDurationSoFar = playerGameState.attackStartTime.getElapsedTime().asMilliseconds();
        if (attackDurationSoFar < SimulationProperties::ATTACK_ANIMATION_DURATION) {
            return false;
        } else {
            playerGameState.attacking = false;
        }
    }

    if (playerGameState.position.y >= SimulationProperties::MAX_Y_BOUNDARY) { // is touching ground
        playerGameState.timeInAir = 0.0f;

        if (command.attack) {
            playerGameState.attacking = true;
            playerGameState.attackStartTime.restart();

            if (playerGameState.direction == SimulationProperties::Direction::LEFT) {
                logger.info("Fire projectile LEFT from (" + std::to_string(playerGameState.position.x) + ", " + std::to_string(playerGameState.position.y) + ").");
            } else {
                logger.info("Fire projectile RIGHT from (" + std::to_string(playerGameState.position.x) + ", " + std::to_string(playerGameState.position.y) + ").");
            }

            clientConnection.queueProjectileCreationBroadcast(playerGameState.position, playerGameState.direction, playerGameState.isPlayer1);

            return false;
        } else if (command.jump) {
            velocity.y = SimulationProperties::JUMP_KICKOFF_VELOCITY;
            playerGameState.timeInAir += deltaTime.asSeconds();
        }
    } else {
        if (playerGameState.timeInAir < SimulationProperties::JUMP_KICK_OFF_TIME) {
            velocity.y = SimulationProperties::JUMP_KICKOFF_VELOCITY;
        } else if (playerGameState.timeInAir < SimulationProperties::MAX_AIR_TIME) {
            velocity.y = SimulationProperties::JUMP_VELOCITY;
        } else {
            velocity.y = SimulationProperties::GRAVITY;
        }
        playerGameState.timeInAir += deltaTime.asSeconds();
    }

    SimulationProperties::Direction newDirection = playerGameState.direction;
    if (command.left) {
        velocity.x -= SimulationProperties::RUN_VELOCITY;
        newDirection = SimulationProperties::Direction::LEFT;
    }

    if (command.right) {
        velocity.x += SimulationProperties::RUN_VELOCITY;
        newDirection = SimulationProperties::Direction::RIGHT;
    }

    if (velocity.x != 0.0f) {
        playerGameState.direction = newDirection;
    }

    playerGameState.position = playerGameState.position + velocity * deltaTime.asSeconds();

    if (playerGameState.position.x < SimulationProperties::MIN_X_BOUNDARY) {
        playerGameState.position.x = SimulationProperties::MIN_X_BOUNDARY;
    } else if (playerGameState.position.x > SimulationProperties::MAX_X_BOUNDARY) {
        playerGameState.position.x = SimulationProperties::MAX_X_BOUNDARY;
    }

    if (playerGameState.position.y > SimulationProperties::MAX_Y_BOUNDARY) {
        playerGameState.position.y = SimulationProperties::MAX_Y_BOUNDARY;
    }

    return velocity.x != 0.0f || velocity.y != 0.0f;
}

void GameSimulation::checkForNetworkUpdates() {
    ClientUpdate player1Update;
    auto player1UpdateStatus = clientConnection.getPlayer1Update(player1Update);
    if (player1UpdateStatus == NonBlockingNetOpStatus::COMPLETE) {
        // logger.info("Received " + std::to_string(player1Update.move.unackedMoveCommands.size()) + " move commands for player 1.");
        if (player1Update.type == ClientUpdate::Type::MOVE) {
            for (auto cmd : player1Update.move.unackedMoveCommands) {
                player1GameState.movementQueue.push(cmd);
            }
        }
    } else if (player1UpdateStatus == NonBlockingNetOpStatus::ERROR) {
        logger.error("An unexpected error occurred while checking for network updates for player 1");
    }

    ClientUpdate player2Update;
    auto player2UpdateStatus = clientConnection.getPlayer2Update(player2Update);
    if (player2UpdateStatus == NonBlockingNetOpStatus::COMPLETE) {
        if (player2Update.type == ClientUpdate::Type::MOVE) {
            // logger.info("Received " + std::to_string(player2Update.move.unackedMoveCommands.size()) + " move commands for player 2.");
            for (auto cmd : player2Update.move.unackedMoveCommands) {
                player2GameState.movementQueue.push(cmd);
            }
        }
    } else if (player1UpdateStatus == NonBlockingNetOpStatus::ERROR) {
        logger.error("An unexpected error occurred while checking for network updates for player 2");
    }
}

void GameSimulation::movePlayers(sf::Time deltaTime) {
    bool player1PositionChanged = movePlayer(player1GameState, deltaTime);
    bool player2PositionChanged = movePlayer(player2GameState, deltaTime);
    clientConnection.broadcastPlayerPositions(time, player1PositionChanged, player1GameState.position, player2PositionChanged, player2GameState.position);
}

void GameSimulation::incrementTime() {
    time++;
}
