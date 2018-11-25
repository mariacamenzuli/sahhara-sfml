#pragma once

#include "NetworkCommunicationSignals.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Utf.hpp>

class AuthoritativeGameUpdate {
public:
    AuthoritativeGameUpdate();
    ~AuthoritativeGameUpdate();

    enum class Type {
        PLAYER_POSITION_UPDATE,
        MOVE_COMMAND_ACK,
        UNKNOWN
    };

    struct PlayerPositionUpdate {
        bool isUpdateForPlayer1;
        sf::Vector2f newPosition;

        PlayerPositionUpdate(bool isUpdateForPlayer1, float x, float y) : isUpdateForPlayer1(isUpdateForPlayer1), newPosition(x, y) {
        }
    };

    struct MoveCommandAckUpdate {
        sf::Uint16 sequenceNumber;

        explicit MoveCommandAckUpdate(sf::Uint16 sequenceNumber)
            : sequenceNumber(sequenceNumber) {
        }
    };

    union {
        PlayerPositionUpdate playerPosition;
        MoveCommandAckUpdate moveCommandAck;
    };

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ServerSignal::PLAYER_POSITION_UPDATE:
            return Type::PLAYER_POSITION_UPDATE;
        case ServerSignal::MOVE_COMMAND_ACK:
            return Type::MOVE_COMMAND_ACK;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};

