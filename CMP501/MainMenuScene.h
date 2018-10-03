#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceManager.h"

#include <SFML/Graphics/Sprite.hpp>

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector, ResourceManager* resourceManager);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	void render(sf::RenderWindow* window) override;
private:
	GameSceneDirector* sceneDirector;
	ResourceManager* resourceManager;
	sf::Sprite backgroundSprite;
};
