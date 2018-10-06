#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class ResourceLoader {
public:
	enum class TextureId {
		BACKGROUND,
		WIZARD_PURPLE
	};

	enum class FontId {
		GAME_TITLE,
		FPS_DISPLAY
	};

	ResourceLoader();
	~ResourceLoader();

	void loadTexture(TextureId textureId, const std::string& filename);
	void releaseTexture(TextureId textureId);
	const sf::Texture& getTexture(TextureId textureId) const;

	void loadFont(FontId fontId, const std::string& filename);
	const sf::Font& getFont(FontId fontId) const;

private:
	std::map<TextureId, std::unique_ptr<sf::Texture>> textureMap;
	std::map<FontId, std::unique_ptr<sf::Font>> fontMap;

	static std::string toString(TextureId textureId);
	static std::string toString(FontId fontId);
};
