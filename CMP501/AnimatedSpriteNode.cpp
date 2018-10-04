#include "AnimatedSpriteNode.h"
#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>

AnimatedSpriteNode::AnimatedSpriteNode(Animation animation): animation(animation), currentFrame(0) {
	sprite.setTexture(*animation.getSpriteSheet());
	sprite.setTextureRect(animation.getFrame(currentFrame));
}

AnimatedSpriteNode::~AnimatedSpriteNode() = default;

void AnimatedSpriteNode::setAnimation(Animation animation) {
	this->animation = animation;
	currentFrame = 0;
	// m_texture = m_animation->getSpriteSheet();
	// m_currentFrame = 0;
	// setFrame(m_currentFrame);
	sprite.setTexture(*animation.getSpriteSheet());
	sprite.setTextureRect(animation.getFrame(currentFrame));
}

void AnimatedSpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
