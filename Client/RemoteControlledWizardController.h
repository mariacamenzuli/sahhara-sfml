#pragma once

#include "WizardController.h"
#include "GameServerConnection.h"

class RemoteControlledWizardController : public WizardController {
public:
    RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer);
    ~RemoteControlledWizardController();

    void simulationUpdate(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) override;
    void considerKnownPosition(sf::Uint16 time, sf::Vector2f knownPosition);
    void updatePredictedPositions(sf::Uint16 currentSimulationTime);
    void interpolatePosition(sf::Time timeSinceLastSimulationUpdate);

private:
    struct RemotePlayerPosition {
        int time;
        sf::Vector2f position;

        RemotePlayerPosition(int time, const sf::Vector2f& position) : time(time), position(position) {
        }
    };

    WizardNode* wizard;
    GameServerConnection* gameServer;
    WizardNode::Direction lastKnownDirection;
    RemotePlayerPosition lastKnownPositions[2];
    sf::Vector2f lastPredictedPositions[2];

    float changeRange(float value, float oldMin, float oldMax, float newMin, float newMax);
};

