#include "ResourceManager.h"

#include <iostream>
#include <cassert>

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager() = default;

void ResourceManager::loadTexture(TextureId textureId, const std::string& filename) {
	std::cout << "Loading texture " << filename << std::endl;
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(filename)) {
		throw std::runtime_error("Failed to load texture " + filename);
	}
	auto inserted = textureMap.insert(std::make_pair(textureId, std::move(texture)));
	assert(inserted.second);
}

const sf::Texture& ResourceManager::getTexture(TextureId textureId) const {
	const auto found = textureMap.find(textureId);
	assert(found != textureMap.end());
	return *found->second;
}

void ResourceManager::loadFont(FontId fontId, const std::string& filename) {
	std::cout << "Loading font " << filename << std::endl;
	std::unique_ptr<sf::Font> font(new sf::Font());
	if (!font->loadFromFile(filename)) {
		throw std::runtime_error("Failed to load font " + filename);
	}
	auto inserted = fontMap.insert(std::make_pair(fontId, std::move(font)));
	assert(inserted.second);
}

const sf::Font& ResourceManager::getFont(FontId fontId) const {
	const auto found = fontMap.find(fontId);
	assert(found != fontMap.end());
	return *found->second;
}
