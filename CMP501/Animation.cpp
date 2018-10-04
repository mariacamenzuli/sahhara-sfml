#include "Animation.h"

Animation::Animation(const sf::Texture& texture): texture(&texture) { }

Animation::~Animation() = default;

void Animation::addFrame(sf::IntRect rect) {
	frames.push_back(rect);
}

const sf::Texture* Animation::getSpriteSheet() const {
	return texture;
}

std::size_t Animation::getSize() const {
	return frames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) const {
	return frames[n];
}
