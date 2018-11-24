#include "RemoteControlledWizardController.h"

RemoteControlledWizardController::RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer) : wizard(wizard), gameServer(gameServer) {
}

RemoteControlledWizardController::~RemoteControlledWizardController() = default;

void RemoteControlledWizardController::update(sf::Time deltaTime, bool isGameInFocus) {
}
