#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "AnimatedSpriteNode.h"
#include "Wizard.h"
#include "GameMetricsTracker.h"
#include "ResourceLoader.h"

class BattleScene : public GameScene {
public:
    explicit BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker);
    ~BattleScene();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
    void update(sf::Time deltaTime) override;
    SceneNode* getRootSceneNode() override;
private:
    GameSceneDirector* sceneDirector;
    ResourceLoader* resourceLoader;
    GameMetricsTracker* gameMetricsTracker;
    std::unique_ptr<SceneNode> rootSceneNode;
    Wizard* wizard1;
    Wizard* wizard2;

    const float gravity = 600.0f;

    void buildScene();
};
