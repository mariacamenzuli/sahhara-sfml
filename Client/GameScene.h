#pragma once

#include "SceneNode.h"

#include <SFML/Window/Keyboard.hpp>

class GameScene {
public:
    virtual ~GameScene() = default;
    virtual void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) = 0;
    virtual void update(sf::Time deltaTime, bool isGameInFocus) = 0;
    virtual SceneNode* getRootSceneNode() = 0;
};
