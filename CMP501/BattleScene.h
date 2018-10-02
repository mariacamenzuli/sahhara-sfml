#pragma once

#include "GameScene.h"

class BattleScene : public GameScene {
public:
	explicit BattleScene(GameSceneDirector* sceneDirector);
	~BattleScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
private:
	GameSceneDirector* sceneDirector;
};