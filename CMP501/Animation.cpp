#include "Animation.h"

Animation::Animation(const sf::Texture& texture): texture(&texture) { }

Animation::~Animation() = default;

void Animation::addFrame(sf::IntRect rect, sf::Vector2f displacement) {
	frames.push_back(Frame(rect, displacement));
}

const sf::Texture* Animation::getSpriteSheet() const {
	return texture;
}

std::size_t Animation::getSize() const {
	return frames.size();
}

const Animation::Frame& Animation::getFrame(std::size_t n) const {
	return frames[n];
}
