#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation {
public:
    explicit Animation(const sf::Texture& texture);
    ~Animation();

    struct Frame {
        sf::IntRect rect;
        sf::Vector2f displacement;

        Frame(sf::IntRect rect, sf::Vector2f displacement) : rect(rect), displacement(displacement) {
        }
    };

    void addFrame(sf::IntRect rect, sf::Vector2f displacement = sf::Vector2f(0.0f, 0.0f));
    const sf::Texture* getSpriteSheet() const;
    std::size_t getSize() const;
    const Frame& getFrame(std::size_t n) const;

private:
    std::vector<Frame> frames;
    const sf::Texture* texture;
};
