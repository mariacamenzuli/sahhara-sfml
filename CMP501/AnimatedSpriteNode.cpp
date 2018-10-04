#include "AnimatedSpriteNode.h"
#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>

AnimatedSpriteNode::AnimatedSpriteNode(const Animation& animation): animation(animation), currentFrame(0) {
	sprite.setTexture(*animation.getSpriteSheet());
	sprite.setTextureRect(animation.getFrame(currentFrame));
}

AnimatedSpriteNode::~AnimatedSpriteNode() = default;

void AnimatedSpriteNode::setAnimation(Animation animation) {
	this->animation = animation;
	currentFrame = 0;
	sprite.setTexture(*animation.getSpriteSheet());
	setFrame(currentFrame);
	timeSinceLastUpdate = sf::Time::Zero;
}

void AnimatedSpriteNode::setFrame(std::size_t newFrame) {
	sprite.setTextureRect(animation.getFrame(currentFrame));
}

void AnimatedSpriteNode::setFrameTime(sf::Time frameTime) {
	this->frameTime = frameTime;
}

void AnimatedSpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

void AnimatedSpriteNode::updateCurrent(sf::Time deltaTime) {
	timeSinceLastUpdate += deltaTime;

	if (timeSinceLastUpdate >= frameTime) {
		timeSinceLastUpdate = sf::microseconds(timeSinceLastUpdate.asMicroseconds() % frameTime.asMicroseconds());
		if (currentFrame + 1 < animation.getSize())
			currentFrame++;
		else {
			currentFrame = 0;
		}
		setFrame(currentFrame);
	}
}
