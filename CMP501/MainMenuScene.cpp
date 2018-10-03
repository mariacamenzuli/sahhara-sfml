#include "MainMenuScene.h"
#include "Label.h"
#include "Image.h"
#include "Camera.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector, ResourceManager* resourceManager) : sceneDirector(sceneDirector), resourceManager(resourceManager), rootGameObject(new SceneNode()) {
	std::unique_ptr<Camera> camera(new Camera(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
	rootGameObject->attachChild(std::move((camera)));

	sf::Sprite backgroundSprite = sf::Sprite(resourceManager->getTexture(ResourceManager::TextureId::BACKGROUND));

	std::unique_ptr<Image> background(new Image(backgroundSprite));
	background->setPosition(0.0f, 0.0f);
	rootGameObject->attachChild(std::move(background));

	sf::Text gameTitleText;
	gameTitleText.setFont(resourceManager->getFont(ResourceManager::FontId::GAME_TITLE));
	gameTitleText.setString("Sahhara");
	gameTitleText.setCharacterSize(250);
	gameTitleText.setFillColor(sf::Color::Black);

	std::unique_ptr<Label> gameTitle(new Label(gameTitleText));
	gameTitle->setPosition(425.0f, 150.0f);
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

SceneNode* MainMenuScene::getRootGameObject() {
	return rootGameObject.get();
}
