#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"
#include "TextNode.h"
#include "GameServer.h"

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(GameSceneDirector* sceneDirector,
						   ResourceLoader* resourceLoader,
						   GameMetricsTracker* gameMetricsTracker,
						   GameServer* gameServer);
	~MainMenuScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update(sf::Time deltaTime) override;
	SceneNode* getRootSceneNode() override;
private:
	enum class State {
		CONNECTING_TO_GAME_LOBBY,
		WAITING_FOR_CHALLENGER
	};

	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	GameMetricsTracker* gameMetricsTracker;
	GameServer* gameServer;
	std::unique_ptr<SceneNode> rootSceneNode;
	State state = State::CONNECTING_TO_GAME_LOBBY;

	void buildScene();
	void initiateConnectionToServerLobby();
	void inline clearConnectingToServerLobbyUi();
	void waitForChallenger();
};
