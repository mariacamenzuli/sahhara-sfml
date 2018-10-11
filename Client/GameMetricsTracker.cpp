#include "GameMetricsTracker.h"

GameMetricsTracker::GameMetricsTracker() : logicUpdatesPerSecond(0),
                                           framesPerSecond(0),
                                           logicUpdateCounter(0),
                                           frameCounter(0) {
}

GameMetricsTracker::~GameMetricsTracker() = default;

void GameMetricsTracker::newLogicUpdate() {
    if (lupsClock.getElapsedTime().asSeconds() >= 1.0f) {
        logicUpdatesPerSecond = logicUpdateCounter;
        logicUpdateCounter = 0;
        lupsClock.restart();
    }

    ++logicUpdateCounter;
}

void GameMetricsTracker::newFrameRendered() {
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        framesPerSecond = frameCounter;
        frameCounter = 0;
        fpsClock.restart();
    }

    ++frameCounter;
}
