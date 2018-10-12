#pragma once

namespace ServerSignal {
    const char FOUND_GAME_MATCH = '~';
    const char GAME_ON = '+';
    const char GAME_OFF = '-';

    const bool IS_PLAYER_1 = true;
    const bool IS_NOT_PLAYER_1 = false;
}

namespace ClientSignal {
    const char READY_FOR_MATCH = '!';
}
