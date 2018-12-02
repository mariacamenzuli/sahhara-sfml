#include "ProjectileNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

ProjectileNode::ProjectileNode(sf::Color color) : projectileShape(circleRadius) {
    projectileShape.setFillColor(color);
}

ProjectileNode::~ProjectileNode() = default;

void ProjectileNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(projectileShape, states);
}
