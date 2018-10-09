#include "GameServer.h"

#include <iostream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

GameServer::GameServer() : lobbyTcpSocket(new sf::TcpSocket()) {
}

GameServer::~GameServer() {
	lobbyTcpSocket->disconnect();
};

bool GameServer::connectToGameLobby() {
	// For some reason, non blocking connect always seems to return status NOT_READY, even if the connect was successful
	// Therefore, call a blocking connect with a 1 microsecond timeout
	const sf::Socket::Status status = lobbyTcpSocket->connect(sf::IpAddress("127.0.0.1"), 53000, sf::microseconds(1));
	if (status == sf::Socket::Done) {
		std::cout << "Connected to game lobby." << std::endl;
		lobbyTcpSocket->setBlocking(false);
		return true;
	} else {
		std::cout << "Attempting to connect to game lobby." << std::endl;
		return false;
	}
}
