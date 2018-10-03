#include "BattleScene.h"

BattleScene::BattleScene(GameSceneDirector* sceneDirector) : sceneDirector(sceneDirector) {}

BattleScene::~BattleScene() = default;

void BattleScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->initiateScene(GameSceneDirector::SceneId::MAIN_MENU);
	}
}

void BattleScene::update() {
}

void BattleScene::render(sf::RenderWindow* window) {
}
