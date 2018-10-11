#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "Wizard.h"
#include "FpsDisplay.h"

#include <SFML/Graphics/Sprite.hpp>

BattleScene::BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker) : sceneDirector(sceneDirector), resourceLoader(resourceLoader), gameMetricsTracker(gameMetricsTracker), rootSceneNode(new EmptySceneNode()) {
	resourceLoader->loadTexture(ResourceLoader::TextureId::WIZARD_PURPLE, "Resources/Sprite Sheets/wizard-purple.png");
	resourceLoader->loadTexture(ResourceLoader::TextureId::WIZARD_ORANGE, "Resources/Sprite Sheets/wizard-orange.png");
	buildScene();
}

BattleScene::~BattleScene() {
	resourceLoader->releaseTexture(ResourceLoader::TextureId::WIZARD_PURPLE);
	resourceLoader->releaseTexture(ResourceLoader::TextureId::WIZARD_ORANGE);
};

void BattleScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->initiateScene(GameSceneDirector::SceneId::MAIN_MENU);
		return;
	}
}

void BattleScene::update(sf::Time deltaTime) {
	sf::Vector2f velocity(0.0f, 0.0f);

	if (wizard1->getPosition().y >= 850.0f) { // is touching ground
		wizard1->timeInAir = 0.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			velocity.y = wizard1->jumpKickOffVelocity;
			wizard1->timeInAir += deltaTime.asSeconds();
		}
	} else {
		if (wizard1->timeInAir < wizard1->jumpKickOffTime) {
			velocity.y = wizard1->jumpKickOffVelocity;
		} else if (wizard1->timeInAir < wizard1->maxAirTime) {
			velocity.y = wizard1->jumpVelocity;
		} else {
			velocity.y = gravity;
		}
		wizard1->timeInAir += deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x -= wizard1->runVelocity;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x += wizard1->runVelocity;
	}

	if (velocity.x == 0.0f) {
		wizard1->idle();
	} else if (velocity.x < 0.0f) {
		wizard1->direction = Wizard::Direction::LEFT;
		wizard1->run();
	} else {
		wizard1->direction = Wizard::Direction::RIGHT;
		wizard1->run();
	}

	auto updatedPosition = wizard1->getPosition() + velocity * deltaTime.asSeconds();

	if (updatedPosition.x < 0.0f) {
		updatedPosition.x = 0.0f;
	} else if (updatedPosition.x > 1780.0f) {
		updatedPosition.x = 1780.0f;
	}

	if (updatedPosition.y > 850.0f) {
		updatedPosition.y = 850.0f;
	}

	wizard1->setPosition(updatedPosition);
}

SceneNode* BattleScene::getRootSceneNode() {
	return rootSceneNode.get();
}

void BattleScene::buildScene() {
	std::unique_ptr<CameraNode> camera(new CameraNode(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
	rootSceneNode->attachChild(std::move((camera)));

	sf::Sprite backgroundSprite = sf::Sprite(*resourceLoader->getTexture(ResourceLoader::TextureId::BACKGROUND));

	std::unique_ptr<SpriteNode> background(new SpriteNode(backgroundSprite));
	background->setPosition(0.0f, 0.0f);
	rootSceneNode->attachChild(std::move(background));

	std::unique_ptr<Wizard> wizard1(new Wizard(Wizard::Color::PURPLE, resourceLoader));
	this->wizard1 = wizard1.get();
	wizard1->setPosition(0.0f, 850.0f);
	wizard1->setScale(0.85f, 0.85f);
	wizard1->direction = Wizard::Direction::RIGHT;
	wizard1->idle();
	rootSceneNode->attachChild(std::move(wizard1));

	std::unique_ptr<Wizard> wizard2(new Wizard(Wizard::Color::ORANGE, resourceLoader));
	this->wizard2 = wizard2.get();
	wizard2->setPosition(1780.0f, 850.0f);
	wizard2->setScale(0.85f, 0.85f);
	wizard2->direction = Wizard::Direction::LEFT;
	wizard2->idle();
	rootSceneNode->attachChild(std::move(wizard2));

	std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
	fpsDisplay->getText()->setFont(*resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
	rootSceneNode->attachChild(std::move((fpsDisplay)));
}
