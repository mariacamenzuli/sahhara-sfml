#pragma once

#include <SFML/System/Clock.hpp>

class GameMetricsTracker {
public:
	GameMetricsTracker();
	~GameMetricsTracker();

	unsigned int logicUpdatesPerSecond;
	unsigned int framesPerSecond;

	void newLogicUpdate();
	void newFrameRendered();

private:
	unsigned int logicUpdateCounter;
	unsigned int frameCounter;
	sf::Clock lupsClock;
	sf::Clock fpsClock;
};
