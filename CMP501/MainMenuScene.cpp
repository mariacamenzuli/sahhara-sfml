#include "MainMenuScene.h"
#include "TextNode.h"
#include "SpriteNode.h"
#include "CameraNode.h"
#include "EmptySceneNode.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader) : sceneDirector(sceneDirector), resourceLoader(resourceLoader), rootGameObject(new EmptySceneNode()) {
	buildScene();
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

void MainMenuScene::buildScene() {
	std::unique_ptr<CameraNode> camera(new CameraNode(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
	rootGameObject->attachChild(std::move((camera)));

	sf::Sprite backgroundSprite = sf::Sprite(resourceLoader->getTexture(ResourceLoader::TextureId::BACKGROUND));

	std::unique_ptr<SpriteNode> background(new SpriteNode(backgroundSprite));
	background->setPosition(0.0f, 0.0f);
	rootGameObject->attachChild(std::move(background));

	sf::Text gameTitleText;
	gameTitleText.setFont(resourceLoader->getFont(ResourceLoader::FontId::GAME_TITLE));
	gameTitleText.setString("Sahhara");
	gameTitleText.setCharacterSize(250);
	gameTitleText.setFillColor(sf::Color::Black);

	std::unique_ptr<TextNode> gameTitle(new TextNode(gameTitleText));
	gameTitle->setPosition(425.0f, 150.0f);
	rootGameObject->attachChild(std::move(gameTitle));
}
