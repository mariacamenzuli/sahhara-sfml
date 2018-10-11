#include "GameObjectNode.h"

GameObjectNode::GameObjectNode() = default;

GameObjectNode::~GameObjectNode() = default;

void GameObjectNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    drawCurrent(target, states);

    for (const auto& child : children) {
        child->draw(target, states);
    }
}
