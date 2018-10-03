#include "GameObject.h"

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	drawCurrent(target, states);

	for (const auto& child : children) {
		child->draw(target, states);
	}
}
