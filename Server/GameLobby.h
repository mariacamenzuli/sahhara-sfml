#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <queue>
#include <memory>

class GameLobby { //todo: add support for a third player?
public:
	GameLobby();
	~GameLobby();

	void run();

private:
	const int maxOngoingGames = 1;
	int ongoingGames = 0;
	std::queue<std::unique_ptr<sf::TcpSocket>> clientsAwaitingGame;
};