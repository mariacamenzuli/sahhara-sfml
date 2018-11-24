#include "LocallyControlledWizardController.h"
#include "BattleScene.h"

#include <SFML/Window/Keyboard.hpp>

LocallyControlledWizardController::LocallyControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer): wizard(wizard), gameServer(gameServer) {
}

LocallyControlledWizardController::~LocallyControlledWizardController() = default;

void LocallyControlledWizardController::update(sf::Time deltaTime, bool isGameInFocus) {
    sf::Vector2f velocity(0.0f, 0.0f);

    if (wizard->getPosition().y >= 865.0f) {
        // is touching ground
        wizard->timeInAir = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isGameInFocus) {
            velocity.y = wizard->jumpKickOffVelocity;
            wizard->timeInAir += deltaTime.asSeconds();
        }
    } else {
        if (wizard->timeInAir < wizard->jumpKickOffTime) {
            velocity.y = wizard->jumpKickOffVelocity;
        } else if (wizard->timeInAir < wizard->maxAirTime) {
            velocity.y = wizard->jumpVelocity;
        } else {
            velocity.y = BattleScene::GRAVITY;
        }
        wizard->timeInAir += deltaTime.asSeconds();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && isGameInFocus) {
        velocity.x -= wizard->runVelocity;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && isGameInFocus) {
        velocity.x += wizard->runVelocity;
    }

    if (velocity.x == 0.0f) {
        wizard->idle();
    } else if (velocity.x < 0.0f) {
        wizard->direction = WizardNode::Direction::LEFT;
        wizard->run();
        gameServer->sendMoveCommand(GameServerConnection::Command::MOVE_LEFT);
    } else {
        wizard->direction = WizardNode::Direction::RIGHT;
        wizard->run();
        gameServer->sendMoveCommand(GameServerConnection::Command::MOVE_RIGHT);
    }

    // Predict new position

    auto updatedPosition = wizard->getPosition() + velocity * deltaTime.asSeconds();

    if (updatedPosition.x < 0.0f) {
        updatedPosition.x = 0.0f;
    } else if (updatedPosition.x > 1780.0f) {
        updatedPosition.x = 1780.0f;
    }

    if (updatedPosition.y > 865.0f) {
        updatedPosition.y = 865.0f;
    }

    wizard->setPosition(updatedPosition);
}
