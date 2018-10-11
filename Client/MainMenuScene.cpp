#include "MainMenuScene.h"
#include "TextNode.h"
#include "SpriteNode.h"
#include "CameraNode.h"
#include "EmptySceneNode.h"
#include "FpsDisplay.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <thread>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector,
                             ResourceLoader* resourceLoader,
                             GameMetricsTracker* gameMetricsTracker,
                             GameServerConnection* gameServer) : sceneDirector(sceneDirector),
                                                                 resourceLoader(resourceLoader),
                                                                 gameMetricsTracker(gameMetricsTracker),
                                                                 gameServer(gameServer),
                                                                 rootSceneNode(new EmptySceneNode()) {
    buildScene();
    initiateConnectionToServerLobby();
}

MainMenuScene::~MainMenuScene() = default;

void MainMenuScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Escape && isPressed) {
        sceneDirector->concludeCurrentScene();
        return;
    }
}

void MainMenuScene::update(sf::Time deltaTime) {
    switch (state) {
    case State::CONNECTING_TO_GAME_LOBBY:
        if (gameServer->connectToGameLobby()) {
            clearConnectingToServerLobbyUi();
            waitForChallenger();
        }
        break;
    case State::WAITING_FOR_CHALLENGER:
        auto gameInfo = gameServer->findGame();
        if (gameInfo.connectionStatus == GameServerConnection::ConnectionStatus::OK) {
            sceneDirector->initiateScene(GameSceneDirector::SceneId::BATTLE);
        } else if (gameInfo.connectionStatus == GameServerConnection::ConnectionStatus::ERROR) {
            clearWaitingForChallengerUi();
            initiateConnectionToServerLobby();
        }
        break;
    }
}

SceneNode* MainMenuScene::getRootSceneNode() {
    return rootSceneNode.get();
}

void MainMenuScene::buildScene() {
    std::unique_ptr<CameraNode> camera(new CameraNode(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
    rootSceneNode->attachChild(std::move((camera)));

    sf::Sprite backgroundSprite = sf::Sprite(*resourceLoader->getTexture(ResourceLoader::TextureId::BACKGROUND));

    std::unique_ptr<SpriteNode> background(new SpriteNode(backgroundSprite));
    background->setPosition(0.0f, 0.0f);
    rootSceneNode->attachChild(std::move(background));

    sf::Text gameTitleText;
    gameTitleText.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TITLE));
    gameTitleText.setString("Sahhara");
    gameTitleText.setCharacterSize(250);
    gameTitleText.setFillColor(sf::Color::Black);

    std::unique_ptr<TextNode> gameTitle(new TextNode(gameTitleText));
    gameTitle->setPosition(465.0f, 150.0f);
    rootSceneNode->attachChild(std::move(gameTitle));

    std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
    fpsDisplay->getText()->setFont(*resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
    rootSceneNode->attachChild(std::move((fpsDisplay)));
}

void MainMenuScene::initiateConnectionToServerLobby() {
    state = State::CONNECTING_TO_GAME_LOBBY;

    sf::Text connectingText;
    connectingText.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    connectingText.setString("Connecting to Server...");
    connectingText.setStyle(sf::Text::Bold);
    connectingText.setCharacterSize(75);
    connectingText.setFillColor(sf::Color::Black);

    std::unique_ptr<TextNode> connectingTextNode(new TextNode(connectingText));
    connectingTextNode->setPosition(685.0f, 525.0f);
    rootSceneNode->attachChild(std::move(connectingTextNode), "CONNECTING_MSG");
}

void MainMenuScene::clearConnectingToServerLobbyUi() {
    rootSceneNode->detachChild(*rootSceneNode->getChild("CONNECTING_MSG"));
}

void MainMenuScene::waitForChallenger() {
    state = State::WAITING_FOR_CHALLENGER;

    sf::Text connectingText;
    connectingText.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    connectingText.setString("Waiting for a Challenger...");
    connectingText.setStyle(sf::Text::Bold);
    connectingText.setCharacterSize(75);
    connectingText.setFillColor(sf::Color::Black);

    std::unique_ptr<TextNode> connectingTextNode(new TextNode(connectingText));
    connectingTextNode->setPosition(635.0f, 525.0f);
    rootSceneNode->attachChild(std::move(connectingTextNode), "WAITING_MSG");
}

void MainMenuScene::clearWaitingForChallengerUi() {
    rootSceneNode->detachChild(*rootSceneNode->getChild("WAITING_MSG"));
}
