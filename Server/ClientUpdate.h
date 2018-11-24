#pragma once
#include "../Client/GameClient.h"

class ClientUpdate {
public:
    ClientUpdate();
    ~ClientUpdate();

    enum class Type {
        MOVE,
        UNKNOWN
    };

    struct MoveUpdate {
        bool left;
        bool right;
        bool jump;

        MoveUpdate(bool left, bool right, bool jump) : left(left), right(right), jump(jump) {
        }
    };

    union {
        MoveUpdate move;
    };

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

