#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"
#include "TextNode.h"

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update(sf::Time deltaTime) override;
	SceneNode* getRootSceneNode() override;
private:
	enum class State {
		PRESENTING_MENU,
		CONNECTING_TO_SERVER,
		WAITING_FOR_CHALLENGER
	};

	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	GameMetricsTracker* gameMetricsTracker;
	std::unique_ptr<SceneNode> rootSceneNode;
	State state = State::PRESENTING_MENU;

	void buildScene();
	void presentMenu();
	void connectToServer();
};
