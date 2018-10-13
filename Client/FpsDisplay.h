#pragma once

#include "TextNode.h"

#include <SFML/System/Clock.hpp>
#include "GameMetricsTracker.h"

class FpsDisplay : public TextNode {
public:
    FpsDisplay(GameMetricsTracker* gameMetricsTracker);
    ~FpsDisplay();

private:
    GameMetricsTracker* gameMetricsTracker;

    void updateCurrent(sf::Time deltaTime, bool isGameInFocus) override;
    void inline updateDisplay();
};
