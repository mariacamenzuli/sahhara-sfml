#pragma once

#include "GameObjectNode.h"

#include <SFML/Graphics/Text.hpp>

class TextNode : public GameObjectNode {
public:
	explicit TextNode(sf::Text text);
	~TextNode();

	sf::Text* getText();

protected:
	sf::Text text;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};
