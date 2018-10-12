#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "AnimatedSpriteNode.h"
#include "WizardNode.h"
#include "GameMetricsTracker.h"
#include "ResourceLoader.h"
#include "WizardController.h"

class BattleScene : public GameScene {
public:
    explicit BattleScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker);
    ~BattleScene();

    static constexpr float GRAVITY = 600.0f;

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
    void update(sf::Time deltaTime) override;
    SceneNode* getRootSceneNode() override;
private:
    GameSceneDirector* sceneDirector;
    ResourceLoader* resourceLoader;
    GameMetricsTracker* gameMetricsTracker;
    std::unique_ptr<SceneNode> rootSceneNode;
    WizardNode* player1Wizard;
    WizardNode* player2Wizard;
    std::unique_ptr<WizardController> localWizardController;

    void buildScene();
};
