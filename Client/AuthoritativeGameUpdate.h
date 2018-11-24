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
        unsigned short serverUdpPort;

        InitUpdate(bool isPlayer1, unsigned short udpPort) : isPlayer1(isPlayer1), serverUdpPort(udpPort) {
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

