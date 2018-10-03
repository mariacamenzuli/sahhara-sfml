#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class ResourceManager {
public:
	enum class TextureId {
		BACKGROUND
	};

	enum class FontId {
		GAME_TITLE
	};

	ResourceManager();
	~ResourceManager();

	void loadTexture(TextureId textureId, const std::string& filename);
	const sf::Texture& getTexture(TextureId textureId) const;


	void loadFont(FontId fontId, const std::string& filename);
	const sf::Font& getFont(FontId fontId) const;

private:
	std::map<TextureId, std::unique_ptr<sf::Texture>> textureMap;
	std::map<FontId, std::unique_ptr<sf::Font>> fontMap;
};
