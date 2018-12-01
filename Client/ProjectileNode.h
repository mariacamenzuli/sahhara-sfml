#pragma once

#include "GameObjectNode.h"

#include <SFML/Graphics/CircleShape.hpp>
#include "SimulationProperties.h"

class ProjectileNode : public GameObjectNode {
public:
    ProjectileNode(sf::Color color);
    ~ProjectileNode();

    void setPosition(float x, float y);

private:
    static constexpr float circleRadius = 10.0;

    sf::CircleShape projectileShape;

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

