#pragma once

#include <memory>
#include <vector>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>

class SceneNode : public sf::Drawable,
				  private sf::NonCopyable {
public:
	typedef std::unique_ptr<SceneNode> SceneNodePointer;

	SceneNode();
	~SceneNode();

	void attachChild(SceneNodePointer child);
	SceneNodePointer detachChild(const SceneNode& node);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time deltaTime);

protected:
	std::vector<SceneNodePointer> children;
	SceneNode* parent;

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void updateCurrent(sf::Time deltaTime);
};
