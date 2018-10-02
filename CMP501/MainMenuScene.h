#pragma once

#include "GameScene.h"
#include <vector>

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
private:
	GameSceneDirector* sceneDirector;
};
