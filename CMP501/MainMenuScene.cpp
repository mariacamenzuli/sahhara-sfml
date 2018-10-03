#include "MainMenuScene.h"
#include "Label.h"
#include "Image.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector, ResourceManager* resourceManager) : sceneDirector(sceneDirector), resourceManager(resourceManager), rootGameObject(new GameObject()) {
	sf::Sprite backgroundSprite = sf::Sprite(resourceManager->getTexture(ResourceManager::TextureId::BACKGROUND));
	backgroundSprite.setPosition(0.0f, 0.0f);

	std::unique_ptr<GameObject> background(new Image(backgroundSprite));
	rootGameObject->attachChild(std::move(background));

	sf::Text gameTitleText;
	gameTitleText.setFont(resourceManager->getFont(ResourceManager::FontId::GAME_TITLE));
	gameTitleText.setString("Sahhara");
	gameTitleText.setCharacterSize(250);
	gameTitleText.setFillColor(sf::Color::Black);
	gameTitleText.setPosition(425.0f, 150.0f);

	std::unique_ptr<GameObject> gameTitle(new Label(gameTitleText));
	rootGameObject->attachChild(std::move(gameTitle));
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

GameObject* MainMenuScene::getRootGameObject() {
	return rootGameObject.get();
}
