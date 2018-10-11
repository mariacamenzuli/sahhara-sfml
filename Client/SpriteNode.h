#pragma once

#include "GameObjectNode.h"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public GameObjectNode {
public:
    explicit SpriteNode(sf::Sprite sprite);
    ~SpriteNode();

    sf::Sprite* getSprite();

private:
    sf::Sprite sprite;

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
