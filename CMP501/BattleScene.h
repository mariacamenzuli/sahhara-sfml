#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include <SFML/Graphics/Texture.hpp>

class ResourceLoader;

class BattleScene : public GameScene {
public:
	explicit BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader);
	~BattleScene();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
	void update() override;
	SceneNode* getRootGameObject() override;
private:
	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	std::unique_ptr<SceneNode> rootGameObject;

	void buildScene();
};
