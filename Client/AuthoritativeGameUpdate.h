#pragma once
#include "NetworkCommunicationSignals.h"

class AuthoritativeGameUpdate {
public:
    AuthoritativeGameUpdate();
    ~AuthoritativeGameUpdate();

    enum class Type {
        INIT,
        UNKNOWN
    };

    struct InitUpdate {
        bool isPlayer1;

        explicit InitUpdate(bool isPlayer1)
            : isPlayer1(isPlayer1) {
        }
    };

    union {
        InitUpdate init;
    };

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ServerSignal::GAME_INIT:
            return Type::INIT;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};

