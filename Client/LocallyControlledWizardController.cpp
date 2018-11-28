#include "LocallyControlledWizardController.h"
#include "BattleScene.h"

#include <SFML/Window/Keyboard.hpp>
#include "SimulationProperties.h"

LocallyControlledWizardController::LocallyControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer): wizard(wizard), gameServer(gameServer) {
}

LocallyControlledWizardController::~LocallyControlledWizardController() = default;

void LocallyControlledWizardController::simulationUpdate(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) {
    sf::Vector2f velocity(0.0f, 0.0f);
    bool leftCmdPressed = false;
    bool rightCmdPressed = false;
    bool jumpCmdPressed = false;
    bool attackCmdPressed = false;

    if (isGameInFocus) {
        leftCmdPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        rightCmdPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        jumpCmdPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        attackCmdPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

        if (leftCmdPressed || rightCmdPressed || jumpCmdPressed || attackCmdPressed) {
            gameServer->sendMoveCommand(leftCmdPressed, rightCmdPressed, jumpCmdPressed, attackCmdPressed);
        }
    }

    if (wizard->attacking) {
        const auto attackDurationSoFar = wizard->attackStartTime.getElapsedTime().asMilliseconds();
        if (attackDurationSoFar < SimulationProperties::ATTACK_ANIMATION_DURATION) {
            return;
        } else {
            wizard->attacking = false;
        }
    }

    if (wizard->getPosition().y >= SimulationProperties::MAX_Y_BOUNDARY) { // is touching ground
        wizard->timeInAir = 0.0f;

        if (attackCmdPressed) {
            wizard->attack();
            wizard->attacking = true;
            wizard->attackStartTime.restart();
            return;
        } else if (jumpCmdPressed) {
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

    if (leftCmdPressed) {
        velocity.x -= SimulationProperties::RUN_VELOCITY;
    }

    if (rightCmdPressed) {
        velocity.x += SimulationProperties::RUN_VELOCITY;
    }

    if (velocity.x == 0.0f) {
        wizard->idle();
    } else if (velocity.x < 0.0f) {
        wizard->direction = SimulationProperties::Direction::LEFT;
        wizard->run();
    } else {
        wizard->direction = SimulationProperties::Direction::RIGHT;
        wizard->run();
    }

    // Predict new position

    auto updatedPosition = wizard->getPosition() + velocity * deltaTime.asSeconds();

    if (updatedPosition.x < SimulationProperties::MIN_X_BOUNDARY) {
        updatedPosition.x = SimulationProperties::MIN_X_BOUNDARY;
    } else if (updatedPosition.x > SimulationProperties::MAX_X_BOUNDARY) {
        updatedPosition.x = SimulationProperties::MAX_X_BOUNDARY;
    }

    if (updatedPosition.y > SimulationProperties::MAX_Y_BOUNDARY) {
        updatedPosition.y = SimulationProperties::MAX_Y_BOUNDARY;
    }

    wizard->setPosition(updatedPosition);
}
