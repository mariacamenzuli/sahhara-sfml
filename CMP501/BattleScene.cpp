#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include "AnimatedSpriteNode.h"

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
	}
}

void BattleScene::update(sf::Time deltaTime) {
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

	Animation deadRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	deadRightAnimation.addFrame(sf::IntRect(0, 174, 174, 165));
	deadRightAnimation.addFrame(sf::IntRect(174, 174, 174, 141), sf::Vector2f(11.0f, 27.0f));
	deadRightAnimation.addFrame(sf::IntRect(348, 174, 174, 141), sf::Vector2f(11.0f, 20.0f));
	deadRightAnimation.addFrame(sf::IntRect(522, 174, 143, 93), sf::Vector2f(15.0f, 70.0f));

	Animation deadLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	deadLeftAnimation.addFrame(sf::IntRect(1436, 174, 174, 165));
	deadLeftAnimation.addFrame(sf::IntRect(1262, 174, 174, 141), sf::Vector2f(-11.0f, 27.0f));
	deadLeftAnimation.addFrame(sf::IntRect(1088, 174, 174, 141), sf::Vector2f(-11.0f, 20.0f));
	deadLeftAnimation.addFrame(sf::IntRect(945, 174, 143, 93), sf::Vector2f(10.0f, 70.0f));

	Animation hurtRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	hurtRightAnimation.addFrame(sf::IntRect(0, 339, 174, 165));
	hurtRightAnimation.addFrame(sf::IntRect(174, 339, 174, 150), sf::Vector2f(13.0f, 18.0f));
	hurtRightAnimation.addFrame(sf::IntRect(348, 339, 174, 165));

	Animation hurtLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	hurtLeftAnimation.addFrame(sf::IntRect(1436, 339, 174, 165));
	hurtLeftAnimation.addFrame(sf::IntRect(1262, 339, 174, 150), sf::Vector2f(-13.0f, 18.0f));
	hurtLeftAnimation.addFrame(sf::IntRect(1088, 339, 174, 165));
	
	Animation runRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runRightAnimation.addFrame(sf::IntRect(0, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(166, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(332, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(498, 663, 166, 156));

	Animation runLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runLeftAnimation.addFrame(sf::IntRect(1444, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(1278, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(1112, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(946, 663, 166, 156));

	Animation jumpRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	jumpRightAnimation.addFrame(sf::IntRect(286, 504, 172, 145));
	jumpRightAnimation.addFrame(sf::IntRect(458, 504, 130, 159));
	jumpRightAnimation.addFrame(sf::IntRect(588, 504, 172, 145));

	Animation jumpLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	jumpLeftAnimation.addFrame(sf::IntRect(1152, 504, 172, 145));
	jumpLeftAnimation.addFrame(sf::IntRect(1022, 504, 130, 159));
	jumpLeftAnimation.addFrame(sf::IntRect(850, 504, 172, 145));

	Animation attackRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	attackRightAnimation.addFrame(sf::IntRect(0, 0, 168, 143));
	attackRightAnimation.addFrame(sf::IntRect(168, 0, 258, 163), sf::Vector2f(9.0f, -20.0f));
	attackRightAnimation.addFrame(sf::IntRect(426, 0, 291, 174), sf::Vector2f(11.0f, -33.0f));

	Animation attackLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	attackLeftAnimation.addFrame(sf::IntRect(1442, 0, 168, 143));
	attackLeftAnimation.addFrame(sf::IntRect(1184, 0, 258, 163), sf::Vector2f(-98.0f, -20.0f));
	attackLeftAnimation.addFrame(sf::IntRect(893, 0, 291, 174), sf::Vector2f(-133.0f, -33.0f));

	Animation idleRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	idleRightAnimation.addFrame(sf::IntRect(522, 339, 143, 150));
	idleRightAnimation.addFrame(sf::IntRect(665, 339, 143, 148), sf::Vector2f(1.0f, 0.0f));
	idleRightAnimation.addFrame(sf::IntRect(0, 504, 143, 148), sf::Vector2f(0.0f, 1.0f));
	idleRightAnimation.addFrame(sf::IntRect(143, 504, 143, 148), sf::Vector2f(2.0f, 1.0f));

	Animation idleLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	idleLeftAnimation.addFrame(sf::IntRect(945, 339, 143, 148));
	idleLeftAnimation.addFrame(sf::IntRect(802, 339, 143, 150), sf::Vector2f(-1.0f, 0.0f));
	idleLeftAnimation.addFrame(sf::IntRect(1467, 504, 143, 148), sf::Vector2f(0.0f, -1.0f));
	idleLeftAnimation.addFrame(sf::IntRect(1324, 504, 143, 148), sf::Vector2f(-2.0f, -1.0f));

	std::unique_ptr<AnimatedSpriteNode> wizard(new AnimatedSpriteNode());
	wizard->addAnimation(WizardAnimations::RUN_RIGHT, runRightAnimation, (sf::seconds(0.133f)));
	wizard->addAnimation(WizardAnimations::RUN_LEFT, runLeftAnimation, (sf::seconds(0.133f)));
	wizard->addAnimation(WizardAnimations::JUMP_RIGHT, jumpRightAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::JUMP_LEFT, jumpLeftAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::ATTACK_RIGHT, attackRightAnimation, (sf::seconds(0.533f)));
	wizard->addAnimation(WizardAnimations::ATTACK_LEFT, attackLeftAnimation, (sf::seconds(0.533f)));
	wizard->addAnimation(WizardAnimations::IDLE_RIGHT, idleRightAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::IDLE_LEFT, idleLeftAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::HURT_RIGHT, hurtRightAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::HURT_LEFT, hurtLeftAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::DEAD_RIGHT, deadRightAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::DEAD_LEFT, deadLeftAnimation, (sf::seconds(0.75f)));
	wizard->setPosition(425.0f, 850.0f);
	wizard->setScale(0.85f, 0.85f);
	wizard->setAnimation(WizardAnimations::DEAD_LEFT);
	rootGameObject->attachChild(std::move(wizard));
}
