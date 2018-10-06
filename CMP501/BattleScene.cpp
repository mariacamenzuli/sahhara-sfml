#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include "AnimatedSpriteNode.h"
#include "Wizard.h"

BattleScene::BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader) : sceneDirector(sceneDirector), resourceLoader(resourceLoader), rootGameObject(new EmptySceneNode()) {
	resourceLoader->loadTexture(ResourceLoader::TextureId::WIZARD_PURPLE, "Resources/Sprite Sheets/wizard-purple.png");
	buildScene();
}

BattleScene::~BattleScene() {
	resourceLoader->releaseTexture(ResourceLoader::TextureId::WIZARD_PURPLE);
};

void BattleScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->initiateScene(GameSceneDirector::SceneId::MAIN_MENU);
		return;
	}
}

void BattleScene::update(sf::Time deltaTime) {
	bool noMovementOccurred = true;
	sf::Vector2f movement(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		noMovementOccurred = false;
		movement.x -= wizard->runVelocity;
		wizard->runLeft();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		noMovementOccurred = false;
		movement.x += wizard->runVelocity;
		wizard->runRight();
	}

	if (noMovementOccurred) {
		wizard->idleRight();
	}

	wizard->move(movement * deltaTime.asSeconds());
}

SceneNode* BattleScene::getRootGameObject() {
	return rootGameObject.get();
}

void BattleScene::buildScene() {
	std::unique_ptr<CameraNode> camera(new CameraNode(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
	rootGameObject->attachChild(std::move((camera)));

	sf::Sprite backgroundSprite = sf::Sprite(resourceLoader->getTexture(ResourceLoader::TextureId::BACKGROUND));

	std::unique_ptr<SpriteNode> background(new SpriteNode(backgroundSprite));
	background->setPosition(0.0f, 0.0f);
	rootGameObject->attachChild(std::move(background));

	std::unique_ptr<Wizard> wizard(new Wizard(resourceLoader));
	this->wizard = wizard.get();
	wizard->setPosition(0.0f, 850.0f);
	wizard->setScale(0.85f, 0.85f);
	wizard->idleRight();
	rootGameObject->attachChild(std::move(wizard));
}
