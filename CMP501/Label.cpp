#include "Label.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

Label::Label(sf::Text text) : text(std::move(text)) { }

Label::~Label() = default;

sf::Text* Label::getText() {
	return &text;
}

void Label::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(text, states);
}
