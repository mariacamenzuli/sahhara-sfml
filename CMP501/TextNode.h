#pragma once

#include "GameObjectNode.h"

#include <SFML/Graphics/Text.hpp>

class TextNode : public GameObjectNode {
public:
	explicit TextNode(sf::Text text);
	~TextNode();

	sf::Text* getText();
private:
	sf::Text text;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
