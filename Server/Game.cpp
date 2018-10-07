#include "Game.h"

Game::Game(sf::IpAddress player1Ip, short player1Port, sf::IpAddress player2Ip, short player2Port): player1Ip(player1Ip), player1Port(player1Port), player2Ip(player2Ip), player2Port(player2Port) {
}

Game::~Game() = default;
