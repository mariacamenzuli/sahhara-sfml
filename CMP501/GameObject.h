#pragma once

#include <memory>
#include <vector>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class GameObject : public sf::Transformable, public sf::Drawable,
				   private sf::NonCopyable {
public:
	typedef std::unique_ptr<GameObject> SceneNodePointer;
	GameObject();
	~GameObject();

	void attachChild(SceneNodePointer child);
	SceneNodePointer detachChild(const GameObject& node);
private:
	std::vector<SceneNodePointer> children;
	GameObject* parent;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};
