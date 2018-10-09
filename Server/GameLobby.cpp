#include "GameLobby.h"
#include "Game.h"
#include "ServerSignal.h"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <iostream>

GameLobby::GameLobby() = default;

GameLobby::~GameLobby() = default;

void GameLobby::run() {
	sf::TcpListener listener;

	if (listener.listen(53000) != sf::Socket::Done) {
		throw std::runtime_error("Failed to start listening on TCP socket 53000.");
	}

	std::cout << "Listening for new game requests on TCP port 53000." << std::endl;

	while (true) {
		std::unique_ptr<sf::TcpSocket> playerConnection(new sf::TcpSocket());
		if (listener.accept(*playerConnection) != sf::Socket::Done) {
			std::cout << "Failed to accept a new TCP connection." << std::endl;
			continue;
		}

		std::cout << "----------------------------------" << std::endl;
		std::cout << "Accepted connection from [" << playerConnection->getRemoteAddress().toString() << ":" << playerConnection->getRemotePort() << "]." << std::endl;

		if (clientsAwaitingGame.empty()) {
			std::cout << "Not enough players to start a game yet." << std::endl;
			std::cout << "Queueing new connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(playerConnection));
		} else if (ongoingGames >= maxOngoingGames) {
			std::cout << "Number of ongoing games is at the maximum allowed." << std::endl;
			std::cout << "Queueing new connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(playerConnection));
		} else {
			sf::TcpSocket* opponentMatched = clientsAwaitingGame.front().get();
			std::cout << "Matched with queueing connection [" << opponentMatched->getRemoteAddress().toString() << ":" << opponentMatched->getRemotePort() << "]." << std::endl;
			std::cout << "Verifying connections." << std::endl;

			const bool opponentReady = isReadyForGame(opponentMatched);
			const bool newPlayerReady = isReadyForGame(playerConnection.get());

			if (opponentReady && newPlayerReady) {
				clientsAwaitingGame.pop();
				std::cout << "Starting game." << std::endl;
				Game game(opponentMatched->getRemoteAddress(), opponentMatched->getRemotePort(), playerConnection->getRemoteAddress(), playerConnection->getRemotePort());
				ongoingGames++;
				//todo: inform client that the game is on
			} else {
				if (!opponentReady) {
					std::cout << "Connection to [" << opponentMatched->getRemoteAddress().toString() << ":" << opponentMatched->getRemotePort() << "] has been lost." << std::endl;
					opponentMatched->disconnect();
					clientsAwaitingGame.pop();
					//todo: inform client that the game is off
				}

				if (!newPlayerReady) {
					std::cout << "Connection to [" << playerConnection->getRemoteAddress().toString() << ":" << playerConnection->getRemotePort() << "] has been lost." << std::endl;
					playerConnection->disconnect();
				} else {
					clientsAwaitingGame.push(std::move(playerConnection));
					//todo: inform client that the game is off
				}
			}
		}
	}
}

bool GameLobby::isReadyForGame(sf::TcpSocket* playerConnection) {
	char dataReceived[1];
	std::size_t received;

	if (playerConnection->send(&SERVER_FOUND_GAME_MATCH_SIGNAL, 1) != sf::Socket::Done) {
		return false;
	}

	if (playerConnection->receive(dataReceived, 1, received) != sf::Socket::Done || dataReceived[0] != CLIENT_READY_FOR_MATCH_SIGNAL) {
		return false;
	}

	return true;
}
