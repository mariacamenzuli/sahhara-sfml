#include "ProjectileHitUpdate.h"

ProjectileHitUpdate::ProjectileHitUpdate(bool hitPlayer1) : hitPlayer1(hitPlayer1) {
}

ProjectileHitUpdate::~ProjectileHitUpdate() = default;

void ProjectileHitUpdate::appendToPacket(sf::Packet& packet) {
    packet << static_cast<sf::Int8>(updateType) << hitPlayer1;
}
