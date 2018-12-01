#pragma once
#include <utility>
#include "../Client/GameClient.h"

class ClientUpdate {
public:
    ClientUpdate();
    ~ClientUpdate();

    enum class Type {
        MOVE,
        PROJECTILE_UPDATE_ACK,
        UNKNOWN
    };

    struct MoveCommand {
        bool left;
        bool right;
        bool jump;
        bool attack;

        MoveCommand(): left(false), right(false), jump(false), attack(false) {
        }

        MoveCommand(bool left, bool right, bool jump, bool attack) : left(left), right(right), jump(jump), attack(attack) {
        }
    };

    struct MoveUpdate {
        sf::Uint16 sequenceNumber;
        std::vector<ClientUpdate::MoveCommand> unackedMoveCommands;

        MoveUpdate(sf::Uint16 sequenceNumber, std::vector<ClientUpdate::MoveCommand> unackedMoveCommands)
            : sequenceNumber(sequenceNumber),
              unackedMoveCommands(std::move(unackedMoveCommands)) {
        }

        MoveUpdate() = default;
    };

    struct ProjectileUpdateAck {
        sf::Uint16 sequenceNumber;

        ProjectileUpdateAck() = default;

        explicit ProjectileUpdateAck(sf::Uint16 sequenceNumber)
            : sequenceNumber(sequenceNumber) {
        }
    };

    MoveUpdate move;
    ProjectileUpdateAck projectileAck;

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ClientSignal::MOVE_COMMAND:
            return Type::MOVE;
        case ClientSignal::PROJECTILE_UPDATE_ACK:
            return Type::PROJECTILE_UPDATE_ACK;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};
