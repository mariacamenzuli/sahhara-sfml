#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameScene {
public:
	virtual ~GameScene() = default;
	virtual void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) = 0;
	virtual void update() = 0;
	virtual void render(sf::RenderWindow* window) = 0;
};
