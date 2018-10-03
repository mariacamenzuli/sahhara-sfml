#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceManager.h"

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector, ResourceManager* resourceManager, sf::Vector2f resolution);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	SceneNode* getRootGameObject() override;
private:
	GameSceneDirector* sceneDirector;
	ResourceManager* resourceManager;
	std::unique_ptr<SceneNode> rootGameObject;
};
