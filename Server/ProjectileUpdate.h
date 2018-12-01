#pragma once

#include <SFML/Network/Packet.hpp>

class ProjectileUpdate {
public:
    virtual ~ProjectileUpdate() = default;
    virtual void appendToPacket(sf::Packet& packet) = 0;
};

