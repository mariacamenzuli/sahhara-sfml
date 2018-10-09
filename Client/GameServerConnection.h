#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <memory>


class GameServerConnection {
public:
	GameServerConnection();
	~GameServerConnection();

	enum class ConnectionStatus {
		NOT_READY, ERROR, OK
	};

	struct GameInitInfo {
		ConnectionStatus connectionStatus;

		explicit GameInitInfo(ConnectionStatus connectionStatus) : connectionStatus(connectionStatus) {
		}
	};

	bool connectToGameLobby();
	GameInitInfo findGame();

private:
	const char serverFoundGameMatchSignal = '~';
	const char clientReadyForMatchSignal = '!';

	int failedLobbyConnectAttempts;
	int failedFindGameAttempts;

	std::unique_ptr<sf::TcpSocket> lobbyTcpSocket;
};
