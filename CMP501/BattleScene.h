#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"

class BattleScene : public GameScene {
public:
	explicit BattleScene(GameSceneDirector* sceneDirector);
	~BattleScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	GameObject* getRootGameObject() override;
private:
	GameSceneDirector* sceneDirector;
	std::unique_ptr<GameObject> rootGameObject;
};