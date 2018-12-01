#pragma once

#include "ProjectileUpdate.h"
#include "NetworkCommunicationSignals.h"

class ProjectileHitUpdate : public ProjectileUpdate {
public:
    ProjectileHitUpdate(bool hitPlayer1);
    ~ProjectileHitUpdate();

    void appendToPacket(sf::Packet& packet) override;

private:
    static const char updateType = ServerSignal::PROJECTILE_HIT;
    bool hitPlayer1;
};

