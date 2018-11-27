#pragma once

#include "WizardController.h"

class StationaryWizardController : public  WizardController {
public:
    StationaryWizardController();
    ~StationaryWizardController();

    void simulationUpdate(sf::Uint16 time, sf::Time deltaTime, bool isGameInFocus) override;
};

