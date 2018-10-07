#include "TextNode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

TextNode::TextNode(sf::Text text) : text(std::move(text)) { }

TextNode::~TextNode() = default;

sf::Text* TextNode::getText() {
	return &text;
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(text, states);
}
