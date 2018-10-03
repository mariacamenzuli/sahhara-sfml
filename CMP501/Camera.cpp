#include "Camera.h"

#include <SFML/Graphics/RenderTarget.hpp>

Camera::Camera(sf::Vector2f resolution, float centerX, float centerY) {
	view.setSize(resolution);
	view.setCenter(centerX, centerY);
};

Camera::~Camera() = default;

void Camera::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(view);
}
