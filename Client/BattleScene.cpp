#include "BattleScene.h"
#include "CameraNode.h"
#include "SpriteNode.h"
#include "ResourceLoader.h"
#include "EmptySceneNode.h"
#include "Wizard.h"
#include "FpsDisplay.h"

#include <SFML/Graphics/Sprite.hpp>

BattleScene::BattleScene(GameSceneDirector* sceneDirector,
                         ResourceLoader* resourceLoader,
                         GameMetricsTracker* gameMetricsTracker) : sceneDirector(sceneDirector),
                                                                   resourceLoader(resourceLoader),
                                                                   gameMetricsTracker(gameMetricsTracker),
                                                                   rootSceneNode(new EmptySceneNode()) {
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
        sceneDirector->transitionToScene(GameSceneDirector::SceneId::MAIN_MENU);
        return;
    }
}

void BattleScene::update(sf::Time deltaTime) {
    sf::Vector2f velocity(0.0f, 0.0f);

    if (player1Wizard->getPosition().y >= 865.0f) {
        // is touching ground
        player1Wizard->timeInAir = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            velocity.y = player1Wizard->jumpKickOffVelocity;
            player1Wizard->timeInAir += deltaTime.asSeconds();
        }
    } else {
        if (player1Wizard->timeInAir < player1Wizard->jumpKickOffTime) {
            velocity.y = player1Wizard->jumpKickOffVelocity;
        } else if (player1Wizard->timeInAir < player1Wizard->maxAirTime) {
            velocity.y = player1Wizard->jumpVelocity;
        } else {
            velocity.y = gravity;
        }
        player1Wizard->timeInAir += deltaTime.asSeconds();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= player1Wizard->runVelocity;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += player1Wizard->runVelocity;
    }

    if (velocity.x == 0.0f) {
        player1Wizard->idle();
    } else if (velocity.x < 0.0f) {
        player1Wizard->direction = Wizard::Direction::LEFT;
        player1Wizard->run();
    } else {
        player1Wizard->direction = Wizard::Direction::RIGHT;
        player1Wizard->run();
    }

    auto updatedPosition = player1Wizard->getPosition() + velocity * deltaTime.asSeconds();

    if (updatedPosition.x < 0.0f) {
        updatedPosition.x = 0.0f;
    } else if (updatedPosition.x > 1780.0f) {
        updatedPosition.x = 1780.0f;
    }

    if (updatedPosition.y > 865.0f) {
        updatedPosition.y = 865.0f;
    }

    player1Wizard->setPosition(updatedPosition);
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

    std::unique_ptr<Wizard> wizard1(new Wizard(Wizard::Color::PURPLE, resourceLoader));
    this->player1Wizard = wizard1.get();
    wizard1->setPosition(0.0f, 865.0f);
    wizard1->setScale(0.85f, 0.85f);
    wizard1->direction = Wizard::Direction::RIGHT;
    wizard1->idle();
    rootSceneNode->attachChild(std::move(wizard1));

    std::unique_ptr<Wizard> wizard2(new Wizard(Wizard::Color::ORANGE, resourceLoader));
    this->player2Wizard = wizard2.get();
    wizard2->setPosition(1780.0f, 865.0f);
    wizard2->setScale(0.85f, 0.85f);
    wizard2->direction = Wizard::Direction::LEFT;
    wizard2->idle();
    rootSceneNode->attachChild(std::move(wizard2));

    std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
    fpsDisplay->getText()->setFont(*resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
    rootSceneNode->attachChild(std::move((fpsDisplay)));
}
