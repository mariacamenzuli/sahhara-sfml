#include "ProjectileController.h"
#include "ProjectileNode.h"
#include <iostream>

ProjectileController::ProjectileController(SceneNode* projectileParentNode): projectileParentNode(projectileParentNode) {
}

ProjectileController::~ProjectileController() = default;

void ProjectileController::addProjectile(bool createdByPlayer1, sf::Vector2f position, SimulationProperties::Direction direction, sf::Uint16 projectileCreationTime, sf::Uint16 currentSimulationTime) {
    std::unique_ptr<ProjectileNode> projectile(new ProjectileNode(createdByPlayer1 ? sf::Color::Magenta : sf::Color::Red));
    ProjectileNode* projectilePointer = projectile.get();
    float yAdjustment = 50.0f;
    float xAdjustment = direction == SimulationProperties::Direction::LEFT ? -10.0f : 120.0f;
    projectilePointer->setPosition(position.x + xAdjustment, position.y + yAdjustment);
    projectileParentNode->attachChild(std::move(projectile));

    //todo fast forward projectile more for defending player
    auto lag = currentSimulationTime - projectileCreationTime;
    // std::cout << lag << std::endl;
    activeProjectiles.emplace_back(projectilePointer, direction, lag > 0 ? lag : 1.0f);
}

void ProjectileController::moveProjectilePositions(sf::Time deltaTime) {
    for (int i = 0; i < activeProjectiles.size(); i++) {
        auto projectile = &activeProjectiles[i];

        sf::Vector2f velocity(0.0f, 0.0f);

        if (projectile->direction == SimulationProperties::Direction::RIGHT) {
            velocity.x += SimulationProperties::PROJECTILE_MOVE_VELOCITY;
        } else {
            velocity.x -= SimulationProperties::PROJECTILE_MOVE_VELOCITY;
        }

        auto updatedPosition = projectile->sceneNode->getPosition() + velocity * (deltaTime.asSeconds() * projectile->fastForward);
        projectile->fastForward = 1;

        if (updatedPosition.x < SimulationProperties::MIN_PROJECTILE_X_BOUNDARY || updatedPosition.x > SimulationProperties::MAX_PROJECTILE_X_BOUNDARY) {
            projectileParentNode->detachChild(projectile->sceneNode);
            activeProjectiles.erase(activeProjectiles.begin() + i);
            i--;
        } else {
            projectile->sceneNode->setPosition(updatedPosition.x, updatedPosition.y);
        }
    }
}
