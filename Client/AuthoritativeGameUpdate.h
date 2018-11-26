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
        bool player1PositionChanged;
        sf::Vector2f newPlayer1Position;
        bool player2PositionChanged;
        sf::Vector2f newPlayer2Position;

        PlayerPositionUpdate(bool player1PositionChanged, const sf::Vector2f& newPlayer1Position, bool player2PositionChanged, const sf::Vector2f& newPlayer2Position)
            : player1PositionChanged(player1PositionChanged),
              newPlayer1Position(newPlayer1Position),
              player2PositionChanged(player2PositionChanged),
              newPlayer2Position(newPlayer2Position) {
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

