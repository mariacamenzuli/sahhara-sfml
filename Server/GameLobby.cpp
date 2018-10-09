#include "GameLobby.h"
#include "Game.h"

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

		//todo: remove dead connections from queue

		std::vector<sf::TcpSocket> vector2;
		if (clientsAwaitingGame.empty()) {
			std::cout << "Not enough players to start a game yet." << std::endl;
			std::cout << "Queueing connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(playerConnection));
		} else if (ongoingGames >= maxOngoingGames) {
			std::cout << "Number of ongoing games is at the maximum allowed." << std::endl;
			std::cout << "Queueing connection for match up." << std::endl;
			clientsAwaitingGame.push(std::move(playerConnection));
		} else {
			// todo: verify opponent connection is still valid
			const auto opponentMatched = std::move(clientsAwaitingGame.front());
			clientsAwaitingGame.pop();
			std::cout << "Matched with queueing connection [" << opponentMatched->getRemoteAddress().toString() << ":" << opponentMatched->getRemotePort() << "]." << std::endl;
			std::cout << "Starting game." << std::endl;
			Game game(opponentMatched->getRemoteAddress(), opponentMatched->getRemotePort(), playerConnection->getRemoteAddress(), playerConnection->getRemotePort());
			ongoingGames++;
		}
	}
}
