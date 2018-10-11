#include "GameClient.h"

#include <iostream>

const char* BANNER =
    R"(
  __ _  __ _ _ __ ___   ___ 
 / _` |/ _` | '_ ` _ \ / _ \
| (_| | (_| | | | | | |  __/
 \__, |\__,_|_| |_| |_|\___|
 |___/                      
      _ _            _      
  ___| (_) ___ _ __ | |_    
 / __| | |/ _ \ '_ \| __|   
| (__| | |  __/ | | | |_    
 \___|_|_|\___|_| |_|\__|   
                            
)";

int main() {
    std::cout << BANNER << std::endl;

    GameClient game;

    try {
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "An error has occurred! Shutting down." << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
