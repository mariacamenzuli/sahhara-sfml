#pragma once

#include "WizardController.h"

class StationaryWizardController : public  WizardController {
public:
    StationaryWizardController();
    ~StationaryWizardController();

    void update(sf::Uint16 time, sf::Time deltaTime, bool isGameInFocus) override;
};

