#include "GameLobby.h"

#include <iostream>

const char * BANNER = R"(
  __ _  __ _ _ __ ___   ___   
 / _` |/ _` | '_ ` _ \ / _ \  
| (_| | (_| | | | | | |  __/  
 \__, |\__,_|_| |_| |_|\___|  
 |___/                        
 ___  ___ _ ____   _____ _ __ 
/ __|/ _ \ '__\ \ / / _ \ '__|
\__ \  __/ |   \ V /  __/ |   
|___/\___|_|    \_/ \___|_|   
                              
)";

int main() {
	std::cout << BANNER << std::endl;

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
