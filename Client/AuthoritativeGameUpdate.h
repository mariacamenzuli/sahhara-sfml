#pragma once

#include "NetworkCommunicationSignals.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Utf.hpp>
#include "SimulationProperties.h"
#include <utility>
#include <vector>

class AuthoritativeGameUpdate {
public:
    AuthoritativeGameUpdate();
    ~AuthoritativeGameUpdate();

    enum class Type {
        PLAYER_POSITION_UPDATE,
        MOVE_COMMAND_ACK,
        PROJECTILE_UPDATE,
        UNKNOWN
    };

    struct PlayerPositionUpdate {
        sf::Uint16 time;
        sf::Vector2f newPlayer1Position;
        sf::Vector2f newPlayer2Position;

        PlayerPositionUpdate() = default;

        PlayerPositionUpdate(sf::Uint16 time,
                             const sf::Vector2f& newPlayer1Position,
                             const sf::Vector2f& newPlayer2Position) : time(time),
                                                                       newPlayer1Position(newPlayer1Position),
                                                                       newPlayer2Position(newPlayer2Position) {
        }
    };

    struct MoveCommandAckUpdate {
        sf::Uint16 sequenceNumber;

        MoveCommandAckUpdate() = default;

        explicit MoveCommandAckUpdate(sf::Uint16 sequenceNumber)
            : sequenceNumber(sequenceNumber) {
        }
    };

    struct ProjectileCreatedUpdate {
        sf::Vector2f position;
        SimulationProperties::Direction direction;
        bool firedByPlayer1;
        sf::Uint16 time;

        ProjectileCreatedUpdate(sf::Uint16 time,
                                const sf::Vector2f& position,
                                SimulationProperties::Direction direction,
                                bool firedByPlayer1) : time(time),
                                                       position(position),
                                                       direction(direction),
                                                       firedByPlayer1(firedByPlayer1) {
        }
    };

    struct ProjectileHitUpdate {
        bool hitPlayer1;

        explicit ProjectileHitUpdate(bool hitPlayer1)
            : hitPlayer1(hitPlayer1) {
        }
    };

    struct ProjectileUpdate {
        sf::Uint16 sequenceNumber;
        std::vector<ProjectileCreatedUpdate> unackedProjectileCreatedUpdates;
        std::vector<ProjectileHitUpdate> unackedProjectileHitUpdates;

        ProjectileUpdate() = default;

        ProjectileUpdate(sf::Uint16 sequenceNumber,
                         std::vector<ProjectileCreatedUpdate> unackedProjectileCreatedUpdates,
                         std::vector<ProjectileHitUpdate> unackedProjectileHitUpdates) : sequenceNumber(sequenceNumber),
                                                                                         unackedProjectileCreatedUpdates(std::move(unackedProjectileCreatedUpdates)),
                                                                                         unackedProjectileHitUpdates(std::move(unackedProjectileHitUpdates)) {
        }

    };

    PlayerPositionUpdate playerPosition;
    MoveCommandAckUpdate moveCommandAck;
    ProjectileUpdate projectile;

    static Type determineUpdateType(char signal) {
        switch (signal) {
        case ServerSignal::PLAYER_POSITION_UPDATE:
            return Type::PLAYER_POSITION_UPDATE;
        case ServerSignal::MOVE_COMMAND_ACK:
            return Type::MOVE_COMMAND_ACK;
        case ServerSignal::PROJECTILE_UPDATE:
            return Type::PROJECTILE_UPDATE;
        default:
            return Type::UNKNOWN;
        }
    }

    Type type;
};
