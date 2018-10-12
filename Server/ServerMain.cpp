#include "GameLobby.h"

#include <iostream>
#include "ThreadLogger.h"

const char* BANNER =
    R"(
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

    ThreadLogger mainThreadLogger("main-thread");
    GameLobby gameLobby(mainThreadLogger);

    try {
        gameLobby.run();
    } catch (const std::exception& e) {
        mainThreadLogger.error("An error has occurred! Shutting down.");
        mainThreadLogger.error(e.what());
        return 1;
    }

    return 0;
}
