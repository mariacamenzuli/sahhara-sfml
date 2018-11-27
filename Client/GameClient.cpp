#include "GameClient.h"

#include "MainMenuScene.h"
#include "BattleScene.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <cassert>

GameClient::GameClient() {
    sf::Vector2f resolution;
    // resolution.x = sf::VideoMode::getDesktopMode().width;
    // resolution.y = sf::VideoMode::getDesktopMode().height;
    resolution.x = 960;
    resolution.y = 540;

    // window.create(sf::VideoMode(resolution.x, resolution.y),
    			  // "Sahhara",
    			  // sf::Style::Fullscreen);

    window.create(sf::VideoMode(resolution.x, resolution.y),
                  "Sahhara");

    window.setFramerateLimit(60);

    resourceLoader.loadImage(ResourceLoader::ImageId::WINDOW_ICON, "Resources/Images/wizard-hat.png");
    resourceLoader.loadTexture(ResourceLoader::TextureId::BACKGROUND, "Resources/Images/desert.png");
    resourceLoader.loadTexture(ResourceLoader::TextureId::GROUND, "Resources/Images/ground.png");
    resourceLoader.loadFont(ResourceLoader::FontId::GAME_TITLE, "Resources/fonts/watermelon-script.ttf");
    resourceLoader.loadFont(ResourceLoader::FontId::FPS_DISPLAY, "Resources/fonts/arial.ttf");
    resourceLoader.loadFont(ResourceLoader::FontId::GAME_TEXT, "Resources/fonts/gabriola.ttf");

    window.setIcon(50, 50, resourceLoader.getImage(ResourceLoader::ImageId::WINDOW_ICON)->getPixelsPtr());

    GameClient::transitionToScene(SceneId::MAIN_MENU);
}

GameClient::~GameClient() = default;

void GameClient::run() {
    sf::Clock clock;
    auto timeSinceLastSimulationUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        processWindowEvents();
        update(timeSinceLastSimulationUpdate);
        timeSinceLastSimulationUpdate += clock.restart();
        while (timeSinceLastSimulationUpdate > timePerSimulationTick) { //todo: run prediction in between simulation ticks?
            timeSinceLastSimulationUpdate -= timePerSimulationTick;
            processWindowEvents();
            simulationUpdate(timePerSimulationTick);
        }
        render();
    }
}

void GameClient::transitionToScene(const SceneId sceneId) {
    switch (sceneId) {
    case SceneId::MAIN_MENU:
        std::cout << "Initiating Main Menu" << std::endl;
        activeScene.reset(new ActiveScene(sceneId, new MainMenuScene(this, &resourceLoader, &gameMetricsTracker, &gameServer)));
        break;
    case SceneId::BATTLE:
        std::cout << "Initiating Battle" << std::endl;
        activeScene.reset(new ActiveScene(sceneId, new BattleScene(this, &resourceLoader, &gameMetricsTracker, &gameServer)));
        break;
    }
}

void GameClient::concludeCurrentScene() {
    if (activeScene->sceneId == SceneId::MAIN_MENU) {
        std::cout << "Main Menu closed. Shutting down." << std::endl;
        window.close();
    }
}

void GameClient::processWindowEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            activeScene->sceneController->handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            activeScene->sceneController->handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

void GameClient::update(sf::Time timeSinceLastSimulationUpdate) {
    activeScene->sceneController->update(timeSinceLastSimulationUpdate);
}

void GameClient::simulationUpdate(sf::Time deltaTime) {
    activeScene->sceneController->simulationUpdate(deltaTime, window.hasFocus());
    assert(activeScene->sceneController->getRootSceneNode());
    activeScene->sceneController->getRootSceneNode()->simulationUpdate(deltaTime, window.hasFocus());
    gameMetricsTracker.newLogicUpdate();
}

void GameClient::render() {
    window.clear();
    assert(activeScene->sceneController->getRootSceneNode());
    window.draw(*activeScene->sceneController->getRootSceneNode());
    window.display();
    gameMetricsTracker.newFrameRendered();
}
