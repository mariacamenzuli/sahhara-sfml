#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

class Game : GameSceneDirector {
public:
	Game();
	~Game();

	void run();
	void initiateScene(const SceneId sceneId) override;
	void concludeCurrentScene() override;
private:
	void processWindowEvents();
	void update();
	void render();
private:
	struct ActiveScene {
		SceneId sceneId;
		std::unique_ptr<GameScene> scene;

		ActiveScene(const SceneId sceneId, GameScene* gameScene): sceneId(sceneId), scene(gameScene) { }
	};

	sf::RenderWindow window;
	std::unique_ptr<ActiveScene> activeScene;
};
