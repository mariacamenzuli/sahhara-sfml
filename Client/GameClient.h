#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"
#include "GameServer.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameClient : GameSceneDirector {
public:
	GameClient();
	~GameClient();

	void run();
	void initiateScene(const SceneId sceneId) override;
	void concludeCurrentScene() override;
private:
	struct ActiveScene {
		SceneId sceneId;
		std::unique_ptr<GameScene> sceneController;

		ActiveScene(const SceneId sceneId, GameScene* gameScene): sceneId(sceneId), sceneController(gameScene) { }
	};

	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::RenderWindow window;
	std::unique_ptr<ActiveScene> activeScene;
	ResourceLoader resourceLoader;
	GameMetricsTracker gameMetricsTracker;
	GameServer gameServer;

	void processWindowEvents();
	void update(sf::Time deltaTime);
	void render();
};
