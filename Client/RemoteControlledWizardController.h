#pragma once

#include "WizardController.h"
#include "GameServerConnection.h"

class RemoteControlledWizardController : public WizardController {
public:
    RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer);
    ~RemoteControlledWizardController();

    void update(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) override;
    void considerKnownPosition(sf::Uint16 time, sf::Vector2f knownPosition);

private:
    struct RemotePlayerPosition {
        int time;
        sf::Vector2f position;

        RemotePlayerPosition(int time, const sf::Vector2f& position) : time(time), position(position) {
        }
    };

    WizardNode* wizard;
    GameServerConnection* gameServer;
    RemotePlayerPosition lastKnownPositions[2];
};

