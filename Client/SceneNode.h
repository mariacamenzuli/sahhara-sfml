#pragma once

#include <memory>
#include <vector>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <map>

class SceneNode : public sf::Drawable,
                  private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> SceneNodePointer;

    SceneNode();
    ~SceneNode();

    SceneNode* attachChild(SceneNodePointer child);
    SceneNode* attachChild(SceneNodePointer child, std::string label);
    SceneNodePointer detachChild(const SceneNode& node);
    SceneNode* getChild(std::string label);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void simulationUpdate(sf::Time deltaTime, bool isGameInFocus);

protected:
    std::vector<SceneNodePointer> children;
    std::map<std::string, SceneNode*> labeledChildren;
    SceneNode* parent;

    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual void updateCurrent(sf::Time deltaTime, bool isGameInFocus);
};
