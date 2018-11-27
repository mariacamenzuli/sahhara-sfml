#pragma once

#include "GameScene.h"
#include "GameSceneDirector.h"
#include "ResourceLoader.h"
#include "GameMetricsTracker.h"
#include "GameServerConnection.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "SimulationProperties.h"

class GameClient : GameSceneDirector {
public:
    GameClient();
    ~GameClient();

    void run();
    void transitionToScene(const SceneId sceneId) override;
    void concludeCurrentScene() override;
private:
    struct ActiveScene {
        SceneId sceneId;
        std::unique_ptr<GameScene> sceneController;

        ActiveScene(const SceneId sceneId, GameScene* gameScene): sceneId(sceneId), sceneController(gameScene) {
        }
    };

    const sf::Time timePerSimulationTick = sf::seconds(1.f / SimulationProperties::TICKS_PER_SECOND);
    sf::RenderWindow window;
    ResourceLoader resourceLoader;
    GameMetricsTracker gameMetricsTracker;
    GameServerConnection gameServer;
    std::unique_ptr<ActiveScene> activeScene;

    void processWindowEvents();
    void update(sf::Time timeSinceLastSimulationUpdate);
    void simulationUpdate(sf::Time deltaTime);
    void render();
};
