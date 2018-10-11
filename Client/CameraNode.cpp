#include "CameraNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

CameraNode::CameraNode(sf::Vector2f resolution, float centerX, float centerY) {
    view.setSize(resolution);
    view.setCenter(centerX, centerY);
};

CameraNode::~CameraNode() = default;

void CameraNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.setView(view);
}
