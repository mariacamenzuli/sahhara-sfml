#pragma once

#include "WizardController.h"
#include "GameServerConnection.h"

class RemoteControlledWizardController : public WizardController {
public:
    RemoteControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer);
    ~RemoteControlledWizardController();

    void update(sf::Time deltaTime, bool isGameInFocus) override;

private:
    WizardNode* wizard;
    GameServerConnection* gameServer;
};

