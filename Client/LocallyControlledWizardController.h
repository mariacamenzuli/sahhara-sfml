#pragma once
#include "WizardController.h"

class LocallyControlledWizardController : public WizardController {
public:
    explicit LocallyControlledWizardController(WizardNode* wizard);
    ~LocallyControlledWizardController();

    void update(sf::Time deltaTime, bool isGameInFocus) override;

private:
    WizardNode* wizard;
};

