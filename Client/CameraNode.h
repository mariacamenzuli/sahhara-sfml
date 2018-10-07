#pragma once

#include "SceneNode.h"

#include <SFML/Graphics/View.hpp>

class CameraNode : public SceneNode {
public:
	CameraNode(sf::Vector2f resolution, float centerX, float centerY);
	~CameraNode();

private:
	sf::View view;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
