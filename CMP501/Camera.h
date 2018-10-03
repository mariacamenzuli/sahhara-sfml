#pragma once

#include "SceneNode.h"

#include <SFML/Graphics/View.hpp>

class Camera : public SceneNode {
public:
	Camera(sf::Vector2f resolution, float centerX, float centerY);
	~Camera();

private:
	sf::View view;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
