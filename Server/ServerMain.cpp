#include "GameLobby.h"

#include <iostream>

int main() {
	GameLobby gameLobby;

	try {
		gameLobby.run();
	} catch (const std::exception& e) {
		std::cerr << "An error has occurred! Shutting down." << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
