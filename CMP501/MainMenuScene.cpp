#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector) : sceneDirector(sceneDirector) {}

MainMenuScene::~MainMenuScene() = default;

void MainMenuScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->concludeCurrentScene();
	} else if (key == sf::Keyboard::Enter && isPressed) {
		sceneDirector->initiateScene(GameSceneDirector::SceneId::BATTLE);
	}
}

void MainMenuScene::update() {
}
