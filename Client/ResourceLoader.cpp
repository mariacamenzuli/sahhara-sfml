#include "ResourceLoader.h"

#include <iostream>
#include <cassert>

ResourceLoader::ResourceLoader() = default;

ResourceLoader::~ResourceLoader() = default;

void ResourceLoader::loadTexture(TextureId textureId, const std::string& filename) {
	std::cout << "Loading texture " << toString(textureId) << " from "<< filename << std::endl;
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(filename)) {
		throw std::runtime_error("Failed to load texture " + filename);
	}
	auto inserted = textureMap.insert(std::make_pair(textureId, std::move(texture)));
	assert(inserted.second);
}

void ResourceLoader::releaseTexture(TextureId textureId) {
	std::cout << "Releasing texture " << toString(textureId) << std::endl;
	textureMap.erase(textureId);
}

const sf::Texture& ResourceLoader::getTexture(TextureId textureId) const {
	const auto found = textureMap.find(textureId);
	assert(found != textureMap.end());
	return *found->second;
}

void ResourceLoader::loadFont(FontId fontId, const std::string& filename) {
	std::cout << "Loading font " << toString(fontId) << " from " << filename << std::endl;
	std::unique_ptr<sf::Font> font(new sf::Font());
	if (!font->loadFromFile(filename)) {
		throw std::runtime_error("Failed to load font " + filename);
	}
	auto inserted = fontMap.insert(std::make_pair(fontId, std::move(font)));
	assert(inserted.second);
}

const sf::Font& ResourceLoader::getFont(FontId fontId) const {
	const auto found = fontMap.find(fontId);
	assert(found != fontMap.end());
	return *found->second;
}

std::string ResourceLoader::toString(TextureId textureId) {
	switch (textureId) {
	case TextureId::BACKGROUND:
		return "BACKGROUND";
	case TextureId::WIZARD_PURPLE:
		return "WIZARD_PURPLE";
	default:
		return "UNKNOWN";
	}
}

std::string ResourceLoader::toString(FontId fontId) {
	switch (fontId) {
	case FontId::GAME_TITLE:
		return "GAME_TITLE";
	default:
		return "UNKNOWN";
	}
}
