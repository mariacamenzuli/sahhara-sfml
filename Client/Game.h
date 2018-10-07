#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Game : GameSceneDirector {
public:
	Game();
	~Game();

	void run();
	void initiateScene(const SceneId sceneId) override;
	void concludeCurrentScene() override;
private:
	void processWindowEvents();
	void update(sf::Time deltaTime);
	void render();
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
};
