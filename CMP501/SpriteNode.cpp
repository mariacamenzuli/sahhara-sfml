#include "SpriteNode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

SpriteNode::SpriteNode(sf::Sprite sprite) : sprite(std::move(sprite)) { }

SpriteNode::~SpriteNode() = default;

sf::Sprite* SpriteNode::getSprite() {
	return &sprite;
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
