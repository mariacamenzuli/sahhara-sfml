#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class ResourceLoader {
public:
    enum class ImageId {
        WINDOW_ICON
    };

    enum class TextureId {
        BACKGROUND,
        WIZARD_PURPLE,
        WIZARD_ORANGE
    };

    enum class FontId {
        GAME_TITLE,
        FPS_DISPLAY,
        GAME_TEXT
    };

    ResourceLoader();
    ~ResourceLoader();

    void loadImage(ImageId imageId, const std::string& filename);
    const sf::Image* getImage(ImageId imageId) const;

    void loadTexture(TextureId textureId, const std::string& filename);
    void releaseTexture(TextureId textureId);
    const sf::Texture* getTexture(TextureId textureId) const;

    void loadFont(FontId fontId, const std::string& filename);
    const sf::Font* getFont(FontId fontId) const;

private:
    std::map<ImageId, std::unique_ptr<sf::Image>> imageMap;
    std::map<TextureId, std::unique_ptr<sf::Texture>> textureMap;
    std::map<FontId, std::unique_ptr<sf::Font>> fontMap;

    static std::string toString(ImageId imageId);
    static std::string toString(TextureId textureId);
    static std::string toString(FontId fontId);
};
