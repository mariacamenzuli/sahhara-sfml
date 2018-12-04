#pragma once

#include "ProjectileUpdate.h"
#include "SimulationProperties.h"
#include "NetworkCommunicationSignals.h"

#include <SFML/System/Vector2.hpp>

class ProjectileCreatedUpdate : public ProjectileUpdate {
public:
    ProjectileCreatedUpdate(sf::Uint16 time, sf::Vector2f position, SimulationProperties::Direction direction, bool firedByPlayer1);
    ~ProjectileCreatedUpdate();

    void appendToPacket(sf::Packet& packet) override;

private:
    static const char updateType = ServerSignal::PROJECTILE_CREATED;
    sf::Vector2f position;
    SimulationProperties::Direction direction;
    bool firedByPlayer1;
    sf::Uint16 time;
};

