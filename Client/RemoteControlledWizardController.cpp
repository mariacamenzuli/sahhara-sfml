#include "RemoteControlledWizardController.h"
#include "SimulationProperties.h"
#include <iostream>

RemoteControlledWizardController::RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer) : wizard(wizard),
                                                                                                                           gameServer(gameServer),
                                                                                                                           lastKnownPositions{
                                                                                                                               RemotePlayerPosition(-1, wizard->getPosition()),
                                                                                                                               RemotePlayerPosition(-2, wizard->getPosition())
                                                                                                                           } {
}

RemoteControlledWizardController::~RemoteControlledWizardController() = default;

void RemoteControlledWizardController::update(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) {
    float slopeX = (lastKnownPositions[0].position.x - lastKnownPositions[1].position.x) / (lastKnownPositions[0].time - lastKnownPositions[1].time);
    float newX = (slopeX * simulationTime) - (slopeX * lastKnownPositions[0].time) + lastKnownPositions[0].position.x;

    float slopeY = (lastKnownPositions[0].position.y - lastKnownPositions[1].position.y) / (lastKnownPositions[0].time - lastKnownPositions[1].time);
    float newY = (slopeY * simulationTime) - (slopeY * lastKnownPositions[0].time) + lastKnownPositions[0].position.y;

    if (newX < SimulationProperties::MIN_X_BOUNDARY) {
        newX = SimulationProperties::MIN_X_BOUNDARY;
    } else if (newX > SimulationProperties::MAX_X_BOUNDARY) {
        newX = SimulationProperties::MAX_X_BOUNDARY;
    }

    if (newY > SimulationProperties::MAX_Y_BOUNDARY) {
        newY = SimulationProperties::MAX_Y_BOUNDARY;
    }

    wizard->setPosition(sf::Vector2f(newX, newY));
}

void RemoteControlledWizardController::considerKnownPosition(sf::Uint16 time, sf::Vector2f knownPosition) {
    if (time > lastKnownPositions[0].time) {
        lastKnownPositions[1] = RemotePlayerPosition(lastKnownPositions[0].time, lastKnownPositions[0].position);
        lastKnownPositions[0] = RemotePlayerPosition(time, knownPosition);
    } else if (time > lastKnownPositions[1].time) {
        lastKnownPositions[1] = RemotePlayerPosition(time, knownPosition);
    }
}
