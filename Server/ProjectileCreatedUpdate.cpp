#include "ProjectileCreatedUpdate.h"

ProjectileCreatedUpdate::ProjectileCreatedUpdate(sf::Vector2f position,
                                                 SimulationProperties::Direction direction,
                                                 bool firedByPlayer1): position(position),
                                                                       direction(direction),
                                                                       firedByPlayer1(firedByPlayer1) {
}

ProjectileCreatedUpdate::~ProjectileCreatedUpdate() = default;

void ProjectileCreatedUpdate::appendToPacket(sf::Packet& packet) {
    packet << static_cast<sf::Int8>(updateType) << position.x << position.y;

    if (direction == SimulationProperties::Direction::RIGHT) {
        packet << ServerSignal::IS_DIRECTION_RIGHT;
    } else {
        packet << ServerSignal::IS_NOT_DIRECTION_RIGHT;
    }

    packet << firedByPlayer1;
}
