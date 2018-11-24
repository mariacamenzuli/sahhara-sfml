#pragma once

#include "NetworkCommunicationSignals.h"

#include <SFML/System/Vector2.hpp>

class AuthoritativeGameUpdate {
public:
    AuthoritativeGameUpdate();
    ~AuthoritativeGameUpdate();

    enum class Type {
        INIT,
        PLAYER_POSITION_UPDATE,
        UNKNOWN
    };

    struct InitUpdate {
        bool isPlayer1;
        unsigned short serverUdpPort;

        InitUpdate(bool isPlayer1, unsigned short udpPort) : isPlayer1(isPlayer1), serverUdpPort(udpPort) {
        }
    };

    struct PlayerPositionUpdate {
        bool isUpdateForPlayer1;
        sf::Vector2f newPosition;

        PlayerPositionUpdate(bool isUpdateForPlayer1, float x, float y) : isUpdateForPlayer1(isUpdateForPlayer1), newPosition(x, y) {
        }
    };

    union {
        InitUpdate init;
        PlayerPositionUpdate playerPosition;
    };

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ServerSignal::GAME_INIT:
            return Type::INIT;
        case ServerSignal::PLAYER_POSITION_UPDATE:
            return Type::PLAYER_POSITION_UPDATE;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};

