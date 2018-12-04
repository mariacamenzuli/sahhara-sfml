#pragma once

#include "SceneNode.h"
#include "SimulationProperties.h"
#include "ProjectileNode.h"

class ProjectileController {
public:
    explicit ProjectileController(SceneNode* projectileParentNode);
    ~ProjectileController();

    void addProjectile(sf::Vector2f position, SimulationProperties::Direction direction, sf::Uint16 projectileCreationTime, sf::Uint16 currentSimulationTime);
    void moveProjectilePositions(sf::Time deltaTime);

private:
    struct Projectile {
        ProjectileNode* sceneNode;
        SimulationProperties::Direction direction;
        short fastForward;

        Projectile(ProjectileNode* sceneNode, SimulationProperties::Direction direction, short fastForward)
            : sceneNode(sceneNode),
              direction(direction), fastForward(fastForward) {
        }
    };

    SceneNode* projectileParentNode;
    std::vector<Projectile> activeProjectiles;
};

