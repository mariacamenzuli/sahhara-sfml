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
	
	Animation runAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runAnimation.addFrame(sf::IntRect(0, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(398, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(796, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(1194, 1593, 398, 375));

	Animation jumpAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	jumpAnimation.addFrame(sf::IntRect(684, 1212, 413, 348));
	jumpAnimation.addFrame(sf::IntRect(1097, 1212, 312, 381));
	jumpAnimation.addFrame(sf::IntRect(1409, 1212, 413, 348));

	Animation attackAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	attackAnimation.addFrame(sf::IntRect(0, 0, 402, 343));
	attackAnimation.addFrame(sf::IntRect(402, 0, 618, 391), sf::Vector2f(19.0f, -48.0f));
	attackAnimation.addFrame(sf::IntRect(1020, 0, 699, 418), sf::Vector2f(23.0f, -78.0f));

	Animation idleAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	idleAnimation.addFrame(sf::IntRect(1251, 815, 342, 360));
	idleAnimation.addFrame(sf::IntRect(1593, 815, 342, 354), sf::Vector2f(3.0f, 0.0f));
	idleAnimation.addFrame(sf::IntRect(0, 1212, 342, 354), sf::Vector2f(1.0f, 2.0f));
	idleAnimation.addFrame(sf::IntRect(342, 1212, 342, 354), sf::Vector2f(4.0f, 2.0f));

	std::unique_ptr<AnimatedSpriteNode> wizard(new AnimatedSpriteNode());
	wizard->addAnimation(WizardAnimations::RUN, runAnimation, (sf::seconds(0.133f)));
	wizard->addAnimation(WizardAnimations::JUMP, jumpAnimation, (sf::seconds(0.75f)));
	wizard->addAnimation(WizardAnimations::ATTACK, attackAnimation, (sf::seconds(0.533f)));
	wizard->addAnimation(WizardAnimations::IDLE, idleAnimation, (sf::seconds(0.75f)));
	wizard->setPosition(425.0f, 850.0f);
	wizard->setScale(0.35f, 0.35f);
	wizard->setAnimation(WizardAnimations::RUN);
	rootGameObject->attachChild(std::move(wizard));
}
