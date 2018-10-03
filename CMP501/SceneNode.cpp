#include "SceneNode.h"

#include <algorithm>
#include <cassert>

SceneNode::SceneNode() = default;

SceneNode::~SceneNode() = default;

void SceneNode::attachChild(SceneNodePointer child) {
	child->parent = this;
	children.push_back(std::move(child));
}

SceneNode::SceneNodePointer SceneNode::detachChild(const SceneNode& node) {
	auto found = std::find_if(children.begin(), children.end(),
							  [&] (SceneNodePointer& child) -> bool { return child.get() == &node; });
	assert(found != children.end());
	SceneNodePointer result = std::move(*found);
	result->parent = nullptr;
	children.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	drawCurrent(target, states);
	for (const auto& child : children) {
		child->draw(target, states);
	}
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}
