#include "Game.h"

#include "MainMenuScene.h"
#include "BattleScene.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <cassert>

Game::Game() : window(sf::VideoMode(1920, 1080), "CMP501") {
	resourceManager.loadTexture(ResourceManager::TextureId::BACKGROUND, "Resources/Images/desert.png");
	resourceManager.loadFont(ResourceManager::FontId::GAME_TITLE, "Resources/fonts/watermelon-script.ttf");
	Game::initiateScene(SceneId::MAIN_MENU);
}

Game::~Game() = default;

void Game::run() {
	sf::Clock clock;
	auto timeSinceLastUpdate = sf::Time::Zero;

	while (window.isOpen()) {
		// processWindowEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			processWindowEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::initiateScene(const SceneId sceneId) {
	switch (sceneId) {
	case SceneId::MAIN_MENU:
		std::cout << "Initiating Main Menu" << std::endl;
		activeScene.reset(new ActiveScene(sceneId, new MainMenuScene(this, &resourceManager)));
		break;
	case SceneId::BATTLE:
		std::cout << "Initiating Battle" << std::endl;
		activeScene.reset(new ActiveScene(sceneId, new BattleScene(this)));
		break;
	}
}

void Game::concludeCurrentScene() {
	if (activeScene->sceneId == SceneId::MAIN_MENU) {
		std::cout << "Main Menu closed. Shutting down." << std::endl;
		window.close();
	}
}

void Game::processWindowEvents() {
	sf::Event event{};
	while (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			activeScene->sceneController->handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			activeScene->sceneController->handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}

void Game::update(sf::Time deltaTime) {
	activeScene->sceneController->update();
}

void Game::render() {
	window.clear();
	assert(activeScene->sceneController->getRootGameObject());
	window.draw(*activeScene->sceneController->getRootGameObject());
	window.display();
}
