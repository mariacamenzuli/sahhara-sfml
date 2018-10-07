#include "MatchMaker.h"

#include <iostream>

int main() {
	MatchMaker matchMaker;

	try {
		matchMaker.run();
	} catch (const std::exception& e) {
		std::cerr << "An error has occurred! Shutting down." << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
