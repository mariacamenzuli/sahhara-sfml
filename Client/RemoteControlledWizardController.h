#pragma once

#include "WizardController.h"
#include "GameServerConnection.h"

class RemoteControlledWizardController : public WizardController {
public:
    RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer);
    ~RemoteControlledWizardController();

    void update(sf::Time deltaTime, bool isGameInFocus) override;
    void setLastKnownPosition(sf::Vector2f lastKnownPosition);

private:
    WizardNode* wizard;
    GameServerConnection* gameServer;
    sf::Vector2f lastKnownPosition;
};

