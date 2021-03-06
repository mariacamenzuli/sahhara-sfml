#pragma once

enum class NonBlockingNetOpStatus {
    NOT_READY,
    ERROR,
    COMPLETE
};

namespace ServerSignal {
    constexpr char PING_TOO_HIGH = 'x';
    constexpr char PING_OK = 'o';
    constexpr char FOUND_GAME_MATCH = '~';
    constexpr char GAME_ON = '+';
    constexpr char GAME_OFF = '-';
    constexpr char GAME_INIT = '0';
    constexpr char MOVE_COMMAND_ACK = '>';
    constexpr char PLAYER_POSITION_UPDATE = '^';
    constexpr char PROJECTILE_UPDATE = 'X';
    constexpr char PROJECTILE_CREATED = 'c';
    constexpr char PROJECTILE_HIT = 'h';

    constexpr bool IS_PLAYER_1 = true;
    constexpr bool IS_NOT_PLAYER_1 = false;

    constexpr bool IS_DIRECTION_RIGHT = true;
    constexpr bool IS_NOT_DIRECTION_RIGHT = false;
}

namespace ClientSignal {
    constexpr char READY_FOR_MATCH = '!';
    constexpr char MOVE_COMMAND = '>';
    constexpr char PROJECTILE_UPDATE_ACK = 'X';
}
