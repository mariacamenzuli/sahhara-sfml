#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"

class BattleScene : public GameScene {
public:
	explicit BattleScene(GameSceneDirector* sceneDirector);
	~BattleScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	void render(sf::RenderWindow* window) override;
private:
	GameSceneDirector* sceneDirector;
};