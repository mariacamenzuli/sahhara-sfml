#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "WizardNode.h"
#include "FpsDisplay.h"
#include "StationaryWizardController.h"
#include "LocallyControlledWizardController.h"
#include "RemoteControlledWizardController.h"

#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

BattleScene::BattleScene(GameSceneDirector* sceneDirector,
                         ResourceLoader* resourceLoader,
                         GameMetricsTracker* gameMetricsTracker,
                         GameServerConnection* gameServer) : sceneDirector(sceneDirector),
                                                             resourceLoader(resourceLoader),
                                                             gameMetricsTracker(gameMetricsTracker),
                                                             gameServer(gameServer),
                                                             rootSceneNode(new EmptySceneNode()),
                                                             localWizardController(new StationaryWizardController()),
                                                             remoteWizardController(new StationaryWizardController()) {
    resourceLoader->loadTexture(ResourceLoader::TextureId::WIZARD_PURPLE, "Resources/Sprite Sheets/wizard-purple.png");
    resourceLoader->loadTexture(ResourceLoader::TextureId::WIZARD_ORANGE, "Resources/Sprite Sheets/wizard-orange.png");
    buildScene();
}

BattleScene::~BattleScene() {
    resourceLoader->releaseTexture(ResourceLoader::TextureId::WIZARD_PURPLE);
    resourceLoader->releaseTexture(ResourceLoader::TextureId::WIZARD_ORANGE);
};

void BattleScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Escape && isPressed) {
        std::cout << "Terminating battle." << std::endl;
        sceneDirector->transitionToScene(GameSceneDirector::SceneId::MAIN_MENU);
        return;
    }
}

void BattleScene::update(sf::Time deltaTime, bool isGameInFocus) {
    AuthoritativeGameUpdate serverUpdate;
    if (gameServer->getGameRunningSocketPort() == 0) { //todo fix
        const auto serverUpdateStatus = gameServer->getAuthoritativeGameUpdate(serverUpdate);

        if (serverUpdateStatus == NonBlockingNetOpStatus::COMPLETE) {
            switch (serverUpdate.type) {
            case AuthoritativeGameUpdate::Type::INIT:
                if (serverUpdate.init.isPlayer1) {
                    std::cout << "Assigned to locally control player 1." << std::endl;
                    localWizardController.reset(new LocallyControlledWizardController(player1Wizard, gameServer));
                    remoteWizardController.reset(new RemoteControlledWizardController(player2Wizard, gameServer));
                } else {
                    std::cout << "Assigned to locally control player 2." << std::endl;
                    remoteWizardController.reset(new RemoteControlledWizardController(player1Wizard, gameServer));
                    localWizardController.reset(new LocallyControlledWizardController(player2Wizard, gameServer));
                }
                gameServer->disconnectFromGameLobby();
                gameServer->setServerGameRunningSocketPort(serverUpdate.init.serverUdpPort);
                break;
            default:
                std::cout << "Received unrecognized update type." << std::endl;
                break;
            }
        } else if (serverUpdateStatus == NonBlockingNetOpStatus::ERROR) {
            std::cout << "Terminating battle." << std::endl;
            sceneDirector->transitionToScene(GameSceneDirector::SceneId::MAIN_MENU);
            return;
        }
    }

    localWizardController->update(deltaTime, isGameInFocus);
}

SceneNode* BattleScene::getRootSceneNode() {
    return rootSceneNode.get();
}

void BattleScene::buildScene() {
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

    std::unique_ptr<WizardNode> wizard1(new WizardNode(WizardNode::Color::PURPLE, resourceLoader));
    this->player1Wizard = wizard1.get();
    wizard1->setPosition(0.0f, 865.0f);
    wizard1->setScale(0.85f, 0.85f);
    wizard1->direction = WizardNode::Direction::RIGHT;
    wizard1->idle();
    rootSceneNode->attachChild(std::move(wizard1));

    std::unique_ptr<WizardNode> wizard2(new WizardNode(WizardNode::Color::ORANGE, resourceLoader));
    this->player2Wizard = wizard2.get();
    wizard2->setPosition(1780.0f, 865.0f);
    wizard2->setScale(0.85f, 0.85f);
    wizard2->direction = WizardNode::Direction::LEFT;
    wizard2->idle();
    rootSceneNode->attachChild(std::move(wizard2));

    std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
    fpsDisplay->getText()->setFont(*resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
    rootSceneNode->attachChild(std::move((fpsDisplay)));
}
