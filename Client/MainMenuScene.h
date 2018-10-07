#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update(sf::Time deltaTime) override;
	SceneNode* getRootSceneNode() override;
private:
	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	GameMetricsTracker* gameMetricsTracker;
	std::unique_ptr<SceneNode> rootSceneNode;

	void buildScene();
};
