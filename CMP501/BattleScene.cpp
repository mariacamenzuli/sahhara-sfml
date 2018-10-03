#include "BattleScene.h"

BattleScene::BattleScene(GameSceneDirector* sceneDirector) : sceneDirector(sceneDirector), rootGameObject(new GameObject()) {}

BattleScene::~BattleScene() = default;

void BattleScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->initiateScene(GameSceneDirector::SceneId::MAIN_MENU);
	}
}

void BattleScene::update() {
}

GameObject* BattleScene::getRootGameObject() {
	return rootGameObject.get();
}
