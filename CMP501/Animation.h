#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation {
public:
	explicit Animation(const sf::Texture& texture);
	~Animation();

	void addFrame(sf::IntRect rect);
	const sf::Texture* getSpriteSheet() const;
	std::size_t getSize() const;
	const sf::IntRect& getFrame(std::size_t n) const;

private:
	std::vector<sf::IntRect> frames;
	const sf::Texture* texture;
};