#include "FpsDisplay.h"

FpsDisplay::FpsDisplay(GameMetricsTracker* gameMetricsTracker) : TextNode(sf::Text()),
                                                                 gameMetricsTracker(gameMetricsTracker) {
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::Black);
    updateDisplay();
}

FpsDisplay::~FpsDisplay() = default;

void FpsDisplay::updateCurrent(sf::Time deltaTime, bool isGameInFocus) {
    updateDisplay();
}

void FpsDisplay::updateDisplay() {
    text.setString("Frames per second: " + std::to_string(gameMetricsTracker->framesPerSecond) + "\nLogic updates per second: " + std::to_string(gameMetricsTracker->logicUpdatesPerSecond));
}
