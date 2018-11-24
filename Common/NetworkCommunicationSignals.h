#pragma once

namespace ServerSignal {
    const char FOUND_GAME_MATCH = '~';
    const char GAME_ON = '+';
    const char GAME_OFF = '-';
    const char GAME_INIT = '0';

    const bool IS_PLAYER_1 = true;
    const bool IS_NOT_PLAYER_1 = false;
}

namespace ClientSignal {
    const char READY_FOR_MATCH = '!';
    const char MOVE_LEFT_COMMAND = '<';
    const char MOVE_RIGHT_COMMAND = '>';
}
