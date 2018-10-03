#include "GameObject.h"

#include <algorithm>
#include <cassert>

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

void GameObject::attachChild(SceneNodePointer child) {
	child->parent = this;
	children.push_back(std::move(child));
}

GameObject::SceneNodePointer GameObject::detachChild(const GameObject& node) {
	auto found = std::find_if(children.begin(), children.end(),
							  [&] (SceneNodePointer& child) -> bool { return child.get() == &node; });
	assert(found != children.end());
	SceneNodePointer result = std::move(*found);
	result->parent = nullptr;
	children.erase(found);
	return result;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	drawCurrent(target, states);
	for (const auto& child : children) {
		child->draw(target, states);
	}
}

void GameObject::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}
