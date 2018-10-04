#pragma once

#include "GameObjectNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>

class AnimatedSpriteNode : public GameObjectNode  {
public:
	explicit AnimatedSpriteNode(const Animation& animation);
	~AnimatedSpriteNode();

	void setAnimation(Animation animation);
	void setFrame(std::size_t newFrame);
	void setFrameTime(sf::Time frameTime);

private:
	Animation animation;
	sf::Sprite sprite;
	std::size_t currentFrame;
	sf::Time frameTime;
	sf::Time timeSinceLastUpdate;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateCurrent(sf::Time deltaTime) override;
};
