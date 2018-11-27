#pragma once

#include "WizardNode.h"

class WizardController {
public:
    virtual ~WizardController() = default;

    virtual void update(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) = 0;
};

