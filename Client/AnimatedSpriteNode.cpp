#include "AnimatedSpriteNode.h"
#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>

AnimatedSpriteNode::AnimatedSpriteNode(): currentFrame(0) {
}

AnimatedSpriteNode::~AnimatedSpriteNode() = default;

void AnimatedSpriteNode::addAnimation(int animationId, const Animation& animation, sf::Time frameTime) {
    animations.insert(std::make_pair(animationId, AnimationConfig(animation, frameTime)));
}

void AnimatedSpriteNode::setAnimation(const int animationId) {
    if (currentAnimationId == animationId) {
        return;
    }
    currentAnimationId = animationId;
    this->currentAnimationConfig = &animations.find(animationId)->second;
    currentFrame = 0;
    sprite.setTexture(*currentAnimationConfig->animation.getSpriteSheet());
    timeSinceLastUpdate = sf::Time::Zero;
    setFrame(currentFrame);
}

void AnimatedSpriteNode::queueAnimation(const int animationId) {
    queuedAnimationId = animationId;
}

void AnimatedSpriteNode::setFrame(std::size_t newFrame) {
    sprite.setTextureRect(currentAnimationConfig->animation.getFrame(currentFrame).rect);
    sprite.setPosition(currentAnimationConfig->animation.getFrame(currentFrame).displacement);
}

void AnimatedSpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

void AnimatedSpriteNode::updateCurrent(sf::Time deltaTime, bool isGameInFocus) {
    timeSinceLastUpdate += deltaTime;

    if (timeSinceLastUpdate >= currentAnimationConfig->frameTime) {
        timeSinceLastUpdate = sf::microseconds(timeSinceLastUpdate.asMicroseconds() % currentAnimationConfig->frameTime.asMicroseconds());
        if (currentFrame + 1 < currentAnimationConfig->animation.getSize())
            currentFrame++;
        else if (queuedAnimationId != -100) {
            setAnimation(queuedAnimationId);
            queuedAnimationId = -100;
            return;
        } else {
            
            currentFrame = 0;
        }
        setFrame(currentFrame);
    }
}
