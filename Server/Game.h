#pragma once

#include <SFML/Network/IpAddress.hpp>

class Game {
public:
	Game(sf::IpAddress player1Ip, short player1Port, sf::IpAddress player2Ip, short player2Port);
	~Game();

private:
	sf::IpAddress player1Ip;
	short player1Port;
	sf::IpAddress player2Ip;
	short player2Port;
};
