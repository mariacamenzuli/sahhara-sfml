#pragma once

#include "SceneNode.h"

#include <SFML/Graphics/Transformable.hpp>

class GameObjectNode : public SceneNode, public sf::Transformable {
public:
    GameObjectNode();
    ~GameObjectNode();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
};

