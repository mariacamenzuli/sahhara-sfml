#pragma once

#include "SceneNode.h"
#include "SimulationProperties.h"
#include "ProjectileNode.h"

class ProjectileController {
public:
    explicit ProjectileController(SceneNode* projectileParentNode);
    ~ProjectileController();

    void addProjectile(sf::Vector2f position, SimulationProperties::Direction direction);
    void moveProjectilePositions(sf::Time deltaTime);

private:
    struct Projectile {
        ProjectileNode* sceneNode;
        SimulationProperties::Direction direction;

        Projectile(ProjectileNode* sceneNode, SimulationProperties::Direction direction)
            : sceneNode(sceneNode),
              direction(direction) {
        }
    };

    SceneNode* projectileParentNode;
    std::vector<Projectile> activeProjectiles;
};

