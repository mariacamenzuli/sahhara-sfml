#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <memory>

class GameServer {
public:
	GameServer();
	~GameServer();

	bool connectToGameLobby();

private:
	int failedLobbyConnectAttempts;

	std::unique_ptr<sf::TcpSocket> lobbyTcpSocket;
};
