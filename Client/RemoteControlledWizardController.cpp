#include "RemoteControlledWizardController.h"
#include "SimulationProperties.h"

RemoteControlledWizardController::RemoteControlledWizardController(WizardNode* wizard,
                                                                   GameServerConnection* gameServer) : wizard(wizard),
                                                                                                       gameServer(gameServer),
                                                                                                       lastKnownDirection(wizard->getPosition().x == 1780 ? SimulationProperties::Direction::LEFT : SimulationProperties::Direction::RIGHT),
                                                                                                       lastKnownPositions{
                                                                                                           RemotePlayerPosition(-1, wizard->getPosition()),
                                                                                                           RemotePlayerPosition(-2, wizard->getPosition())
                                                                                                       },
                                                                                                       lastPredictedPositions{
                                                                                                           sf::Vector2f(wizard->getPosition()),
                                                                                                           sf::Vector2f(wizard->getPosition())
                                                                                                       } {
}

RemoteControlledWizardController::~RemoteControlledWizardController() = default;

void RemoteControlledWizardController::simulationUpdate(sf::Uint16 simulationTime, sf::Time deltaTime, bool isGameInFocus) {
    float deltaX = lastPredictedPositions[0].x - wizard->getPosition().x;

    wizard->direction = lastKnownDirection;

    if (deltaX == 0.0f) {
        wizard->idle();
    } else if (deltaX < 0.0f) {
        wizard->run();
    } else {
        wizard->run();
    }
}

void RemoteControlledWizardController::considerKnownPosition(sf::Uint16 time, sf::Vector2f knownPosition) {
    if (time > lastKnownPositions[0].time) {
        lastKnownPositions[1] = RemotePlayerPosition(lastKnownPositions[0].time, lastKnownPositions[0].position);
        lastKnownPositions[0] = RemotePlayerPosition(time, knownPosition);
    } else if (time > lastKnownPositions[1].time && time < lastKnownPositions[0].time) {
        lastKnownPositions[1] = RemotePlayerPosition(time, knownPosition);
    }

    auto deltaX = lastKnownPositions[0].position.x - lastKnownPositions[1].position.x;
    if (deltaX > 0.001) {
        lastKnownDirection = SimulationProperties::Direction::RIGHT;
    } else if (deltaX < -0.001) {
        lastKnownDirection = SimulationProperties::Direction::LEFT;
    }
}

void RemoteControlledWizardController::updatePredictedPositions(sf::Uint16 currentSimulationTime) {
    int simulationTimeAdjustedForLag = currentSimulationTime - (currentSimulationTime - lastKnownPositions[0].time);

    sf::Vector2f predictionBasedOnLastKnownPositions;
    float slopeX = (lastKnownPositions[0].position.x - lastKnownPositions[1].position.x) / (lastKnownPositions[0].time - lastKnownPositions[1].time);
    predictionBasedOnLastKnownPositions.x = (slopeX * simulationTimeAdjustedForLag) - (slopeX * lastKnownPositions[0].time) + lastKnownPositions[0].position.x;

    float slopeY = (lastKnownPositions[0].position.y - lastKnownPositions[1].position.y) / (lastKnownPositions[0].time - lastKnownPositions[1].time);
    predictionBasedOnLastKnownPositions.y = (slopeY * simulationTimeAdjustedForLag) - (slopeY * lastKnownPositions[0].time) + lastKnownPositions[0].position.y;

    if (predictionBasedOnLastKnownPositions.x < SimulationProperties::MIN_X_BOUNDARY) {
        predictionBasedOnLastKnownPositions.x = SimulationProperties::MIN_X_BOUNDARY;
    } else if (predictionBasedOnLastKnownPositions.x > SimulationProperties::MAX_X_BOUNDARY) {
        predictionBasedOnLastKnownPositions.x = SimulationProperties::MAX_X_BOUNDARY;
    }

    if (predictionBasedOnLastKnownPositions.y > SimulationProperties::MAX_Y_BOUNDARY) {
        predictionBasedOnLastKnownPositions.y = SimulationProperties::MAX_Y_BOUNDARY;
    }

    lastPredictedPositions[1] = sf::Vector2f(lastPredictedPositions[0]);
    lastPredictedPositions[0] = predictionBasedOnLastKnownPositions;
}

void RemoteControlledWizardController::interpolatePosition(sf::Time timeSinceLastSimulationUpdate) {
    sf::Time timePerSimulationTick = sf::seconds(1.f / SimulationProperties::TICKS_PER_SECOND);
    
    float t;
    if (timeSinceLastSimulationUpdate > timePerSimulationTick) {
        t = 1.0f;
    } else {
        t = changeRange(timeSinceLastSimulationUpdate.asMicroseconds(), 0.0f, timePerSimulationTick.asMicroseconds(), 0.0f, 1.0f);
    }
    
    wizard->setPosition(lastPredictedPositions[1] + (t * (lastPredictedPositions[0] - lastPredictedPositions[1])));
}

float RemoteControlledWizardController::changeRange(float value, float oldMin, float oldMax, float newMin, float newMax) {
    float oldRange = oldMax - oldMin;
    float newRange = newMax - newMin;
    return (((value - oldMin) * newRange) / oldRange) + newMin;
}
