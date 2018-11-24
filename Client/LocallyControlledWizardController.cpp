#include "LocallyControlledWizardController.h"
#include "BattleScene.h"

#include <SFML/Window/Keyboard.hpp>
#include "SimulationProperties.h"

LocallyControlledWizardController::LocallyControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer): wizard(wizard), gameServer(gameServer) {
}

LocallyControlledWizardController::~LocallyControlledWizardController() = default;

void LocallyControlledWizardController::update(sf::Time deltaTime, bool isGameInFocus) {
    sf::Vector2f velocity(0.0f, 0.0f);

    if (wizard->getPosition().y >= 865.0f) {
        // is touching ground
        wizard->timeInAir = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isGameInFocus) {
            velocity.y = SimulationProperties::JUMP_KICKOFF_VELOCITY;
            wizard->timeInAir += deltaTime.asSeconds();
        }
    } else {
        if (wizard->timeInAir < SimulationProperties::JUMP_KICK_OFF_TIME) {
            velocity.y = SimulationProperties::JUMP_KICKOFF_VELOCITY;
        } else if (wizard->timeInAir < SimulationProperties::MAX_AIR_TIME) {
            velocity.y = SimulationProperties::JUMP_VELOCITY;
        } else {
            velocity.y = SimulationProperties::GRAVITY;
        }
        wizard->timeInAir += deltaTime.asSeconds();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && isGameInFocus) {
        velocity.x -= SimulationProperties::RUN_VELOCITY;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && isGameInFocus) {
        velocity.x += SimulationProperties::RUN_VELOCITY;
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

    if (updatedPosition.x < SimulationProperties::MIN_X_BOUNDARY) {
        updatedPosition.x = SimulationProperties::MIN_X_BOUNDARY;
    } else if (updatedPosition.x > SimulationProperties::MAX_X_BOUNDARY) {
        updatedPosition.x = SimulationProperties::MAX_X_BOUNDARY;
    }

    if (updatedPosition.y > 865.0f) {
        updatedPosition.y = 865.0f;
    }

    wizard->setPosition(updatedPosition);
}
