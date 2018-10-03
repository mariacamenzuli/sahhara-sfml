#pragma once

#include "GameObject.h"

#include <SFML/Graphics/Sprite.hpp>

class Image : public GameObject {
public:
	Image(sf::Sprite sprite);
	~Image();

	sf::Sprite* getSprite();

private:
	sf::Sprite sprite;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
