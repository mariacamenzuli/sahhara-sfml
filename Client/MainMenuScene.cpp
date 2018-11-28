#include "MainMenuScene.h"
#include "TextNode.h"
#include "SpriteNode.h"
#include "CameraNode.h"
#include "EmptySceneNode.h"
#include "FpsDisplay.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector,
                             ResourceLoader* resourceLoader,
                             GameMetricsTracker* gameMetricsTracker,
                             GameServerConnection* gameServer) : sceneDirector(sceneDirector),
                                                                 resourceLoader(resourceLoader),
                                                                 gameMetricsTracker(gameMetricsTracker),
                                                                 gameServer(gameServer),
                                                                 rootSceneNode(new EmptySceneNode()) {
    buildScene();
    state = State::CONNECTING_TO_GAME_LOBBY;
    showConnectingToServerLobbyUi();
}

MainMenuScene::~MainMenuScene() = default;

void MainMenuScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Escape && isPressed) {
        sceneDirector->concludeCurrentScene();
        return;
    }
}

void MainMenuScene::update(sf::Time timeSinceLastSimulationUpdate) {
    NonBlockingNetOpStatus operationStatus;
    switch (state) {
    case State::CONNECTING_TO_GAME_LOBBY:
        operationStatus = gameServer->connectToGameLobby();
        if (operationStatus == NonBlockingNetOpStatus::COMPLETE) {
            state = State::TESTING_PING;
        } else if (operationStatus == NonBlockingNetOpStatus::ERROR) {
            clearConnectingToServerLobbyUi();
            state = State::FAILED_TO_CONNECT_TO_GAME_LOBBY;
            displayFailedToConnectError();
        }
        break;
    case State::TESTING_PING:
        operationStatus = gameServer->pingCheck();
        if (operationStatus == NonBlockingNetOpStatus::COMPLETE) {
            clearConnectingToServerLobbyUi();
            state = State::WAITING_FOR_GAME_MATCH;
            waitForChallenger();
        } else if (operationStatus == NonBlockingNetOpStatus::ERROR) {
            clearConnectingToServerLobbyUi();
            state = State::FAILED_TO_CONNECT_TO_GAME_LOBBY;
            displayPingTooHighError();
        }
        break;
    case State::WAITING_FOR_GAME_MATCH:
        operationStatus = gameServer->findGame();
        if (operationStatus == NonBlockingNetOpStatus::COMPLETE) {
            state = State::ACCEPTING_GAME_MATCH;
        } else if (operationStatus == NonBlockingNetOpStatus::ERROR) {
            clearWaitingForChallengerUi();
            state = State::CONNECTING_TO_GAME_LOBBY;
            showConnectingToServerLobbyUi();
        }
        break;
    case State::ACCEPTING_GAME_MATCH:
        operationStatus = gameServer->acceptGame();
        if (operationStatus == NonBlockingNetOpStatus::COMPLETE) {
            state = State::WAITING_FOR_GAME_GO_AHEAD;
        } else if (operationStatus == NonBlockingNetOpStatus::ERROR) {
            clearWaitingForChallengerUi();
            state = State::CONNECTING_TO_GAME_LOBBY;
            showConnectingToServerLobbyUi();
        }
        break;
    case State::WAITING_FOR_GAME_GO_AHEAD:
        bool gameOn;
        operationStatus = gameServer->verifyGameLaunch(&gameOn);
        if (operationStatus == NonBlockingNetOpStatus::COMPLETE) {
            if (gameOn) {
                sceneDirector->transitionToScene(GameSceneDirector::SceneId::BATTLE);
            } else {
                state = State::WAITING_FOR_GAME_MATCH;
            }
        } else if (operationStatus == NonBlockingNetOpStatus::ERROR) {
            clearWaitingForChallengerUi();
            state = State::CONNECTING_TO_GAME_LOBBY;
            showConnectingToServerLobbyUi();
        }
    }
}

void MainMenuScene::simulationUpdate(sf::Time deltaTime, bool isGameInFocus) {
    // no-op
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
    
    sf::Sprite groundSprite = sf::Sprite(*resourceLoader->getTexture(ResourceLoader::TextureId::GROUND));

    std::unique_ptr<SpriteNode> ground(new SpriteNode(groundSprite));
    ground->setPosition(0.0f, 983.0f);
    rootSceneNode->attachChild(std::move(ground));

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

void MainMenuScene::showConnectingToServerLobbyUi() {
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

void MainMenuScene::displayFailedToConnectError() {
    sf::Text failedToConnectMsg;
    failedToConnectMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    failedToConnectMsg.setString("Failed to establish server connection.");
    failedToConnectMsg.setStyle(sf::Text::Bold);
    failedToConnectMsg.setCharacterSize(75);
    failedToConnectMsg.setFillColor(sf::Color::Red);
    failedToConnectMsg.setOutlineColor(sf::Color::Black);
    failedToConnectMsg.setOutlineThickness(2);

    sf::Text reasonMsg;
    reasonMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    reasonMsg.setString("The server may be down.");
    reasonMsg.setStyle(sf::Text::Bold);
    reasonMsg.setCharacterSize(75);
    reasonMsg.setFillColor(sf::Color::Red);
    reasonMsg.setOutlineColor(sf::Color::Black);
    reasonMsg.setOutlineThickness(2);

    sf::Text actionMsg;
    actionMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    actionMsg.setString("Please try again later.");
    actionMsg.setStyle(sf::Text::Bold);
    actionMsg.setCharacterSize(75);
    actionMsg.setFillColor(sf::Color::Red);
    actionMsg.setOutlineColor(sf::Color::Black);
    actionMsg.setOutlineThickness(2);

    std::unique_ptr<TextNode> failedToConnectMsgNode(new TextNode(failedToConnectMsg));
    failedToConnectMsgNode->setPosition(535.0f, 525.0f);
    rootSceneNode->attachChild(std::move(failedToConnectMsgNode));

    std::unique_ptr<TextNode> reasonMsgNode(new TextNode(reasonMsg));
    reasonMsgNode->setPosition(735.0f, 625.0f);
    rootSceneNode->attachChild(std::move(reasonMsgNode));

    std::unique_ptr<TextNode> actionMsgNode(new TextNode(actionMsg));
    actionMsgNode->setPosition(735.0f, 725.0f);
    rootSceneNode->attachChild(std::move(actionMsgNode));
}

void MainMenuScene::displayPingTooHighError() {
    sf::Text failedToConnectMsg;
    failedToConnectMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    failedToConnectMsg.setString("Failed to establish server connection.");
    failedToConnectMsg.setStyle(sf::Text::Bold);
    failedToConnectMsg.setCharacterSize(75);
    failedToConnectMsg.setFillColor(sf::Color::Red);
    failedToConnectMsg.setOutlineColor(sf::Color::Black);
    failedToConnectMsg.setOutlineThickness(2);

    sf::Text reasonMsg;
    reasonMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    reasonMsg.setString("Your connection has very high latency.");
    reasonMsg.setStyle(sf::Text::Bold);
    reasonMsg.setCharacterSize(75);
    reasonMsg.setFillColor(sf::Color::Red);
    reasonMsg.setOutlineColor(sf::Color::Black);
    reasonMsg.setOutlineThickness(2);

    sf::Text actionMsg;
    actionMsg.setFont(*resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
    actionMsg.setString("Please try again later.");
    actionMsg.setStyle(sf::Text::Bold);
    actionMsg.setCharacterSize(75);
    actionMsg.setFillColor(sf::Color::Red);
    actionMsg.setOutlineColor(sf::Color::Black);
    actionMsg.setOutlineThickness(2);

    std::unique_ptr<TextNode> failedToConnectMsgNode(new TextNode(failedToConnectMsg));
    failedToConnectMsgNode->setPosition(535.0f, 525.0f);
    rootSceneNode->attachChild(std::move(failedToConnectMsgNode));

    std::unique_ptr<TextNode> reasonMsgNode(new TextNode(reasonMsg));
    reasonMsgNode->setPosition(535.0f, 625.0f);
    rootSceneNode->attachChild(std::move(reasonMsgNode));

    std::unique_ptr<TextNode> actionMsgNode(new TextNode(actionMsg));
    actionMsgNode->setPosition(735.0f, 725.0f);
    rootSceneNode->attachChild(std::move(actionMsgNode));
}

void MainMenuScene::clearWaitingForChallengerUi() {
    rootSceneNode->detachChild(*rootSceneNode->getChild("WAITING_MSG"));
}
