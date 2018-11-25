#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"
#include "TextNode.h"
#include "GameServerConnection.h"

class MainMenuScene : public GameScene {
public:
    explicit MainMenuScene(GameSceneDirector* sceneDirector,
                           ResourceLoader* resourceLoader,
                           GameMetricsTracker* gameMetricsTracker,
                           GameServerConnection* gameServer);
    ~MainMenuScene();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) override;
    void update() override;
    void simulationUpdate(sf::Time deltaTime, bool isGameInFocus) override;
    SceneNode* getRootSceneNode() override;
private:
    enum class State {
        CONNECTING_TO_GAME_LOBBY,
        WAITING_FOR_GAME_MATCH,
        ACCEPTING_GAME_MATCH,
        WAITING_FOR_GAME_GO_AHEAD
    };

    GameSceneDirector* sceneDirector;
    ResourceLoader* resourceLoader;
    GameMetricsTracker* gameMetricsTracker;
    GameServerConnection* gameServer;
    std::unique_ptr<SceneNode> rootSceneNode;
    State state = State::CONNECTING_TO_GAME_LOBBY;

    void buildScene();

    void showConnectingToServerLobbyUi();
    void inline clearConnectingToServerLobbyUi();
    void waitForChallenger();
    void inline clearWaitingForChallengerUi();
};
