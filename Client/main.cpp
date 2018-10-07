#include "Game.h"

#include <iostream>

int main() {
	Game game;

	try {
		game.run();
	} catch (const std::exception& e) {
		std::cerr << "An error has occurred! Shutting down." << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
