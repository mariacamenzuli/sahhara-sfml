#include "Game.h"

#include "MainMenuScene.h"
#include "BattleScene.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

Game::Game() : window(sf::VideoMode(1920, 1080), "CMP501") {
	Game::initiateScene(SceneId::MAIN_MENU);
}

Game::~Game() = default;

void Game::run() {
	while (window.isOpen()) {
		processWindowEvents();
		update();
		render();
	}
}

void Game::initiateScene(const SceneId sceneId) {
	switch (sceneId) {
	case SceneId::MAIN_MENU:
		std::cout << "Initiating Main Menu" << std::endl;
		activeScene.reset(new ActiveScene(sceneId, new MainMenuScene(this)));
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
			activeScene->scene->handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			activeScene->scene->handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}

void Game::update() {
	activeScene->scene->update();
}

void Game::render() {
	window.clear();
	window.display();
}
