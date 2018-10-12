#pragma once

#include "WizardNode.h"

class WizardController {
public:
    virtual ~WizardController() {}

    virtual void update(sf::Time deltaTime) = 0;
};

