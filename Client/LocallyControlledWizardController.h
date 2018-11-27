#pragma once

#include "WizardController.h"
#include "GameServerConnection.h"

class LocallyControlledWizardController : public WizardController {
public:
    LocallyControlledWizardController(WizardNode* wizard, GameServerConnection* gameServer);
    ~LocallyControlledWizardController();

    void update(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) override;

private:
    WizardNode* wizard;
    GameServerConnection* gameServer;
};

