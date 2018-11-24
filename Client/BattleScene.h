#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "AnimatedSpriteNode.h"
#include "WizardNode.h"
#include "GameMetricsTracker.h"
#include "ResourceLoader.h"
#include "WizardController.h"
#include "GameServerConnection.h"

class BattleScene : public GameScene {
public:
    explicit BattleScene(GameSceneDirector* sceneDirector,
                         ResourceLoader* resourceLoader,
                         GameMetricsTracker* gameMetricsTracker,
                         GameServerConnection* gameServer);
    ~BattleScene();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
    void update(sf::Time deltaTime, bool isGameInFocus) override;
    SceneNode* getRootSceneNode() override;
private:
    GameSceneDirector* sceneDirector;
    ResourceLoader* resourceLoader;
    GameMetricsTracker* gameMetricsTracker;
    GameServerConnection* gameServer;
    std::unique_ptr<SceneNode> rootSceneNode;
    WizardNode* player1Wizard;
    WizardNode* player2Wizard;
    std::unique_ptr<WizardController> localWizardController;
    std::unique_ptr<WizardController> remoteWizardController;

    void buildScene();
};
