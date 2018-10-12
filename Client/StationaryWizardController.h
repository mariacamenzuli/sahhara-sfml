#pragma once

#include "WizardController.h"

class StationaryWizardController : public  WizardController {
public:
    StationaryWizardController();
    ~StationaryWizardController();

    void update(sf::Time deltaTime) override;
};

