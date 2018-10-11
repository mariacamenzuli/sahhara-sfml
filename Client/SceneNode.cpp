#include "SceneNode.h"

#include <algorithm>
#include <cassert>

SceneNode::SceneNode() = default;

SceneNode::~SceneNode() = default;

SceneNode* SceneNode::attachChild(SceneNodePointer child) {
    child->parent = this;
    children.push_back(std::move(child));
    return children.at(children.size() - 1).get();
}

SceneNode* SceneNode::attachChild(SceneNodePointer child, std::string label) {
    auto childPtr = this->attachChild(std::move(child));
    labeledChildren.insert(std::make_pair(label, childPtr));
    return childPtr;

}

SceneNode::SceneNodePointer SceneNode::detachChild(const SceneNode& node) {
    auto found = std::find_if(children.begin(), children.end(),
                              [&](SceneNodePointer& child) -> bool { return child.get() == &node; });
    assert(found != children.end());
    SceneNodePointer result = std::move(*found);
    result->parent = nullptr;
    children.erase(found);
    return result;
}

SceneNode* SceneNode::getChild(std::string label) {
    const auto found = labeledChildren.find(label);
    if (found == labeledChildren.end()) {
        return nullptr;
    }
    return found->second;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    drawCurrent(target, states);
    for (const auto& child : children) {
        child->draw(target, states);
    }
}

void SceneNode::update(sf::Time deltaTime) {
    updateCurrent(deltaTime);
    for (const auto& child : children) {
        child->update(deltaTime);
    }
}

void SceneNode::updateCurrent(sf::Time deltaTime) {
    // no-op
}
