#pragma once

#include "GameObject.h"

#include <SFML/Graphics/Text.hpp>

class Label : public GameObject {
public:
	explicit Label(sf::Text text);
	~Label();

	sf::Text* getText();
private:
	sf::Text text;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
