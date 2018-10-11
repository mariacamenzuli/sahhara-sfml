#include "GameLobby.h"
#include "Game.h"
#include "ServerSignal.h"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

GameLobby::GameLobby() = default;

GameLobby::~GameLobby() {
	lobbyListenerSocket.close();
	while (!clientsAwaitingGame.empty()) {
		sf::TcpSocket* clientInQueue = clientsAwaitingGame.front().get();
		clientInQueue->disconnect();
		clientsAwaitingGame.pop();
	}
}

void GameLobby::run() {
	if (lobbyListenerSocket.listen(53000) != sf::Socket::Done) {
		throw std::runtime_error("Failed to start listening on TCP socket 53000.");
	}

	std::cout << "Listening for new game requests on TCP port 53000." << std::endl;

	while (true) {
		std::unique_ptr<sf::TcpSocket> newPlayerConnection(new sf::TcpSocket());
		if (lobbyListenerSocket.accept(*newPlayerConnection) != sf::Socket::Done) {
			std::cout << "Failed to accept a new TCP connection." << std::endl;
			continue;
		}

		std::cout << "----------------------------------" << std::endl;
		std::cout << "Accepted connection from [" << newPlayerConnection->getRemoteAddress().toString() << ":" << newPlayerConnection->getRemotePort() << "]." << std::endl;

		if (clientsAwaitingGame.empty()) {
			std::cout << "Not enough players to start a game yet." << std::endl;
			std::cout << "Queueing new connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(newPlayerConnection));
		} else if (ongoingGames >= maxOngoingGames) {
			std::cout << "Number of ongoing games is at the maximum allowed." << std::endl;
			std::cout << "Queueing new connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(newPlayerConnection));
		} else {
			sf::TcpSocket* opponentMatched = clientsAwaitingGame.front().get();
			std::cout << "Matched with queueing connection [" << opponentMatched->getRemoteAddress().toString() << ":" << opponentMatched->getRemotePort() << "]." << std::endl;
			std::cout << "Verifying connections." << std::endl;

			const bool opponentReady = isReadyForGame(opponentMatched);
			const bool newPlayerReady = isReadyForGame(newPlayerConnection.get());

			if (opponentReady && newPlayerReady) {
				std::cout << "Starting game." << std::endl;

				sf::Packet gameOnPacketPlayer1;
				gameOnPacketPlayer1 << &SERVER_SIGNAL_GAME_ON << SERVER_SIGNAL_IS_PLAYER_1;
				opponentMatched->send(gameOnPacketPlayer1); //todo handle error

				sf::Packet gameOnPacketPlayer2;
				gameOnPacketPlayer2 << &SERVER_SIGNAL_GAME_ON << SERVER_SIGNAL_IS_NOT_PLAYER_1;
				newPlayerConnection->send(gameOnPacketPlayer2); //todo handle error

				clientsAwaitingGame.pop();
				Game game(opponentMatched->getRemoteAddress(), opponentMatched->getRemotePort(), newPlayerConnection->getRemoteAddress(), newPlayerConnection->getRemotePort());
				ongoingGames++;
			} else {
				if (!opponentReady) {
					std::cout << "Connection to [" << opponentMatched->getRemoteAddress().toString() << ":" << opponentMatched->getRemotePort() << "] has been lost." << std::endl;
					opponentMatched->disconnect();
					clientsAwaitingGame.pop();
				} else {
					sf::Packet gameOffPacket;
					gameOffPacket << &SERVER_SIGNAL_GAME_OFF;
					opponentMatched->send(gameOffPacket);
				}

				if (!newPlayerReady) {
					std::cout << "Connection to [" << newPlayerConnection->getRemoteAddress().toString() << ":" << newPlayerConnection->getRemotePort() << "] has been lost." << std::endl;
					newPlayerConnection->disconnect();
				} else {
					sf::Packet gameOffPacket;
					gameOffPacket << &SERVER_SIGNAL_GAME_OFF;
					newPlayerConnection->send(gameOffPacket);

					clientsAwaitingGame.push(std::move(newPlayerConnection));
				}
			}
		}
	}
}

bool GameLobby::isReadyForGame(sf::TcpSocket* playerConnection) {
	char dataReceived[1];
	std::size_t received;

	if (playerConnection->send(&SERVER_SIGNAL_FOUND_GAME_MATCH, 1) != sf::Socket::Done) {
		return false;
	}

	if (playerConnection->receive(dataReceived, 1, received) != sf::Socket::Done || dataReceived[0] != CLIENT_SIGNAL_READY_FOR_MATCH) {
		return false;
	}

	return true;
}
