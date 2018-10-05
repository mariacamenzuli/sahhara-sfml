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
	void update(sf::Time deltaTime) override;
	SceneNode* getRootGameObject() override;
private:
	GameSceneDirector* sceneDirector;
	ResourceLoader* resourceLoader;
	std::unique_ptr<SceneNode> rootGameObject;

	void buildScene();
};
namespace WizardAnimations {
	enum {
		RUN_RIGHT, JUMP_RIGHT, ATTACK_RIGHT, IDLE_RIGHT, HURT_RIGHT, DEAD_RIGHT,
		RUN_LEFT, JUMP_LEFT, ATTACK_LEFT, IDLE_LEFT, HURT_LEFT, DEAD_LEFT
	};
}
