#pragma once

#include "SceneNode.h"

#include <SFML/Graphics/Transformable.hpp>

class GameObject : public SceneNode, public sf::Transformable {
public:
	GameObject();
	~GameObject();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
};

