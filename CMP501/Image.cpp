#include "Image.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

Image::Image(sf::Sprite sprite) : sprite(std::move(sprite)) { }

Image::~Image() = default;

sf::Sprite* Image::getSprite() {
	return &sprite;
}

void Image::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
