#pragma once
#include "SceneNode.h"

class EmptySceneNode : public SceneNode {
public:
    EmptySceneNode();
    ~EmptySceneNode();

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

