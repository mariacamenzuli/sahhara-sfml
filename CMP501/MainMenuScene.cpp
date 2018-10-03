#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector, ResourceManager* resourceManager) : sceneDirector(sceneDirector), resourceManager(resourceManager) {
	backgroundSprite = sf::Sprite(resourceManager->getTexture(ResourceManager::TextureId::BACKGROUND));
	backgroundSprite.setPosition(0.0f, 0.0f);
}

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

void MainMenuScene::render(sf::RenderWindow* window) {
	window->draw(backgroundSprite);
}
