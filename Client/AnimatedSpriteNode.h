#pragma once

#include "GameObjectNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include <map>

class AnimatedSpriteNode : public GameObjectNode {
public:
    AnimatedSpriteNode();
    ~AnimatedSpriteNode();

    void addAnimation(int animationId, const Animation& animation, sf::Time frameTime);
    void setAnimation(const int animationId);
    void setFrame(std::size_t newFrame);

private:
    struct AnimationConfig {
        Animation animation;
        sf::Time frameTime;

        AnimationConfig(const Animation& animation, const sf::Time& frameTime) : animation(animation), frameTime(frameTime) {
        }
    };

    std::map<int, AnimationConfig> animations;
    int currentAnimationId = -100;
    AnimationConfig* currentAnimationConfig;
    sf::Sprite sprite;
    std::size_t currentFrame;
    sf::Time timeSinceLastUpdate;

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time deltaTime, bool isGameInFocus) override;
};
