#include "GameServerConnection.h"

#include <iostream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

GameServerConnection::GameServerConnection() : failedLobbyConnectAttempts(0), failedFindGameAttempts(0),
                                               lobbyTcpSocket(new sf::TcpSocket()) {
}

GameServerConnection::~GameServerConnection() {
	lobbyTcpSocket->disconnect();
};

bool GameServerConnection::connectToGameLobby() {
	lobbyTcpSocket->setBlocking(true);
	const sf::Socket::Status status = lobbyTcpSocket->connect(sf::IpAddress("127.0.0.1"), 53000, sf::microseconds(1));
	if (status == sf::Socket::Done) {
		std::cout << "Connected to game lobby." << std::endl;
		lobbyTcpSocket->setBlocking(false);
		return true;
	} else {
		if (failedLobbyConnectAttempts % 120 == 0) {
			std::cout << "Attempting to connect to game lobby..." << std::endl;
		}
		failedLobbyConnectAttempts++;
		return false;
	}
}

GameServerConnection::GameInitInfo GameServerConnection::findGame() {
	if (failedFindGameAttempts % 120 == 0) {
		std::cout << "Waiting for a game match..." << std::endl;
	}

	char dataReceived[1];
	std::size_t receiveCount;
	const auto serverDataReceiveStatus = lobbyTcpSocket->receive(dataReceived, 1, receiveCount);

	if (serverDataReceiveStatus == sf::Socket::Error) {
		failedFindGameAttempts++;
		std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
		lobbyTcpSocket->disconnect();
		return GameInitInfo(ConnectionStatus::ERROR);
	} else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
		failedFindGameAttempts++;
		std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
		lobbyTcpSocket->disconnect();
		return GameInitInfo(ConnectionStatus::ERROR);
	} else if (serverDataReceiveStatus != sf::Socket::Done) {
		failedFindGameAttempts++;
		return GameInitInfo(ConnectionStatus::NOT_READY);
	}

	if (dataReceived[0] != serverFoundGameMatchSignal) {
		failedFindGameAttempts++;
		std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
		lobbyTcpSocket->disconnect();
		return GameInitInfo(ConnectionStatus::ERROR);
	}

	std::cout << "Game match found." << std::endl;
	std::cout << "Initializing game..." << std::endl;

	std::size_t sendCount = 0;
	while (sendCount != 1) {
		const auto clientDataSentStatus = lobbyTcpSocket->send(&clientReadyForMatchSignal, 1, sendCount);

		if (clientDataSentStatus == sf::Socket::Error) {
			failedFindGameAttempts++;
			std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
			lobbyTcpSocket->disconnect();
			return GameInitInfo(ConnectionStatus::ERROR);
		} else if (clientDataSentStatus == sf::Socket::Disconnected) {
			failedFindGameAttempts++;
			std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
			lobbyTcpSocket->disconnect();
			return GameInitInfo(ConnectionStatus::ERROR);
		}
	}

	// todo: receive game init info (player starting positions, etc)
	std::cout << "Game initialized." << std::endl;

	return GameInitInfo(ConnectionStatus::OK);
}
