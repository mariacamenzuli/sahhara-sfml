#include "RemoteControlledWizardController.h"

RemoteControlledWizardController::RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer) : wizard(wizard), gameServer(gameServer) {
    lastKnownPosition = wizard->getPosition();
}

RemoteControlledWizardController::~RemoteControlledWizardController() = default;

void RemoteControlledWizardController::update(sf::Time deltaTime, bool isGameInFocus) {
    wizard->setPosition(lastKnownPosition);
}

void RemoteControlledWizardController::setLastKnownPosition(sf::Vector2f lastKnownPosition) {
    this->lastKnownPosition = lastKnownPosition;
}
