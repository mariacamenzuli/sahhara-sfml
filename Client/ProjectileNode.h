#pragma once

#include "GameObjectNode.h"

#include <SFML/Graphics/CircleShape.hpp>

class ProjectileNode : public GameObjectNode {
public:
    ProjectileNode(sf::Color color);
    ~ProjectileNode();

private:
    static constexpr float circleRadius = 10.0;

    sf::CircleShape projectileShape;

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

