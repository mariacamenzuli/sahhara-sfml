#pragma once
#include "WizardController.h"

class LocallyControlledWizardController : public WizardController {
public:
    LocallyControlledWizardController(WizardNode* wizard);
    ~LocallyControlledWizardController();

    void update(sf::Time deltaTime) override;

private:
    WizardNode* wizard;
};

