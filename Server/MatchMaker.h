#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <queue>
#include <memory>

class MatchMaker {
public:
	MatchMaker();
	~MatchMaker();

	void run();

private:
	std::queue<std::unique_ptr<sf::TcpSocket>> clientsAwaitingGame;
};
