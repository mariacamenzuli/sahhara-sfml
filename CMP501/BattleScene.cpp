#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "Wizard.h"
#include "FpsDisplay.h"

#include <SFML/Graphics/Sprite.hpp>

BattleScene::BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker) : sceneDirector(sceneDirector), resourceLoader(resourceLoader), gameMetricsTracker(gameMetricsTracker), rootGameObject(new EmptySceneNode()) {
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
	sf::Vector2f velocity(0.0f, 0.0f);

	if (wizard->getPosition().y >= 850.0f) { // is touching ground
		wizard->timeInAir = 0.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			velocity.y = wizard->jumpKickOffVelocity;
			wizard->timeInAir += deltaTime.asSeconds();
		}
	} else {
		if (wizard->timeInAir < wizard->jumpKickOffTime) {
			velocity.y = wizard->jumpKickOffVelocity;
		} else if (wizard->timeInAir < wizard->maxAirTime) {
			velocity.y = wizard->jumpVelocity;
		} else {
			velocity.y = gravity;
		}
		wizard->timeInAir += deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x -= wizard->runVelocity;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x += wizard->runVelocity;
	}

	if (velocity.x == 0.0f) {
		wizard->idle();
	} else if (velocity.x < 0.0f) {
		wizard->direction = Wizard::LEFT;
		wizard->run();
	} else {
		wizard->direction = Wizard::RIGHT;
		wizard->run();
	}

	auto updatedPosition = wizard->getPosition() + velocity * deltaTime.asSeconds();

	if (updatedPosition.x < 0.0f) {
		updatedPosition.x = 0.0f;
	} else if (updatedPosition.x > 1780.0f) {
		updatedPosition.x = 1780.0f;
	}

	if (updatedPosition.y > 850.0f) {
		updatedPosition.y = 850.0f;
	}

	wizard->setPosition(updatedPosition);
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
	wizard->idle();
	rootGameObject->attachChild(std::move(wizard));

	std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
	fpsDisplay->getText()->setFont(resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
	rootGameObject->attachChild(std::move((fpsDisplay)));
}
