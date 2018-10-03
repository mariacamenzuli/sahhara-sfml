#pragma once

#include "GameObject.h"

#include <SFML/Window/Keyboard.hpp>

class GameScene {
public:
	virtual ~GameScene() = default;
	virtual void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) = 0;
	virtual void update() = 0;
	virtual GameObject* getRootGameObject() = 0;
};
