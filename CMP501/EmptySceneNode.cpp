#include "EmptySceneNode.h"

EmptySceneNode::EmptySceneNode() = default;


EmptySceneNode::~EmptySceneNode() = default;

void EmptySceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}
