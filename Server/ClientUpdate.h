#pragma once
#include <utility>
#include "../Client/GameClient.h"

class ClientUpdate {
public:
    ClientUpdate();
    ~ClientUpdate();

    enum class Type {
        MOVE,
        UNKNOWN
    };

    struct MoveCommand {
        bool left;
        bool right;
        bool jump;

        MoveCommand(): left(false), right(false), jump(false) {
        }

        MoveCommand(bool left, bool right, bool jump) : left(left), right(right), jump(jump) {
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

    MoveUpdate move;

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ClientSignal::MOVE_COMMAND:
            return Type::MOVE;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};
