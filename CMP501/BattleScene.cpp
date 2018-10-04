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

void BattleScene::update() {
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

	//todo: https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite
	
	Animation runAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runAnimation.addFrame(sf::IntRect(0, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(398, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(796, 1593, 398, 375));
	runAnimation.addFrame(sf::IntRect(1194, 1593, 398, 375));

	std::unique_ptr<AnimatedSpriteNode> wizard(new AnimatedSpriteNode(runAnimation));
	wizard->setPosition(425.0f, 850.0f);
	wizard->setScale(0.35f, 0.35f);
	rootGameObject->attachChild(std::move(wizard));
}
