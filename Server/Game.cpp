#include "Game.h"
#include "ServerSignal.h"

#include <SFML/Network/Packet.hpp>

Game::Game(std::unique_ptr<sf::TcpSocket> player1TcpConnection,
           std::unique_ptr<sf::TcpSocket> player2TcpConnection) : player1TcpConnection(std::move(player1TcpConnection)),
                                                                  player2TcpConnection(std::move(player2TcpConnection)) {
}

Game::~Game() {
    player1TcpConnection->disconnect();
    player2TcpConnection->disconnect();
};

void Game::run() {
    initialize();
}

void Game::initialize() {
    // sf::Packet gameOnPacketPlayer1;
    // gameOnPacketPlayer1.append(&SERVER_SIGNAL_GAME_ON, 1);
    // gameOnPacketPlayer1 << SERVER_SIGNAL_IS_PLAYER_1;
    // player1TcpConnection->send(gameOnPacketPlayer1); //todo handle error
    //
    // sf::Packet gameOnPacketPlayer2;
    // gameOnPacketPlayer2.append(&SERVER_SIGNAL_GAME_ON, 1);
    // gameOnPacketPlayer2 << SERVER_SIGNAL_IS_NOT_PLAYER_1;
    // player2TcpConnection->send(gameOnPacketPlayer2); //todo handle error
}
