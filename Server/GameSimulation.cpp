#include "GameSimulation.h"
#include "NetworkCommunicationSignals.h"

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

    while (!gameShouldEnd) {
        
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
    char player1GameInitData[2];
    player1GameInitData[0] = ServerSignal::GAME_INIT;
    player1GameInitData[1] = ServerSignal::IS_PLAYER_1;
    player1TcpConnection->send(player1GameInitData, 2); //todo handle error

    char player2GameInitData[2];
    player2GameInitData[0] = ServerSignal::GAME_INIT;
    player2GameInitData[1] = ServerSignal::IS_NOT_PLAYER_1;
    player2TcpConnection->send(player2GameInitData, 2); //todo handle error
}
