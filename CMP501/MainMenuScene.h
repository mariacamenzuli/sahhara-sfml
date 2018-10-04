#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	SceneNode* getRootGameObject() override;
private:
	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	std::unique_ptr<SceneNode> rootGameObject;

	void buildScene();
};
