#include "pch.h"
#include "ResourceSystem.h"
#include <Systems/Logger.h>

namespace Engine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSwooth)
	{
		if (textures.find(name) != textures.end())
		{
			return;
		}

		sf::Texture* newTexture = new sf::Texture;
		if (!newTexture->loadFromFile(sourcePath))
		{
			LOG_ERROR("Failed to load texture '" + name + "' from: " + sourcePath);
			delete newTexture;
			return;
		}
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSwooth);
			textures.emplace(name, newTexture);
		}
	}

	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		return textures.find(name)->second;
	}

	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		return new sf::Texture(*textures.find(name)->second);
	}

	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);

		sf::Texture* deletingtexture = texturePair->second;
		textures.erase(texturePair);
		delete deletingtexture;
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (texturesMaps.find(name) != texturesMaps.end())
		{
			return;
		}

		sf::Texture textureMap;

		if (textureMap.loadFromFile(sourcePath))
		{
			auto textureMapElements = new std::vector<sf::Texture*>();

			auto textureSize = textureMap.getSize();
			int LoadedElement = 0;

			for (int y = 0; y < textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
			{
				if (LoadedElement == totalElements)
				{
					break;
				}
				for (int x = 0; x < textureSize.x - elementPixelSize.x; y += elementPixelSize.x)
				{
					if (LoadedElement == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements->push_back(newTextureMapElement);
					}
					LoadedElement++;
				}
			}
			texturesMaps.emplace(name, *textureMapElements);
		}
	}

	void ResourceSystem::LoadFont(const std::string& name, const std::string& path)
	{
		if (fonts.find(name) != fonts.end())
		{
			LOG_WARN("Font '" + name + "' is already loaded");
			return;
		}

		// Пытаемся загрузить шрифт
		sf::Font* newFont = new sf::Font();
		if (newFont->loadFromFile(path))
		{
			fonts.emplace(name, newFont);
			LOG_INFO("Font '" + name + "' loaded successfully from: " + path);
		}
		else
		{
			delete newFont;
			LOG_ERROR("Failed to load font '" + name + "' from: " + path);
			throw std::runtime_error("Failed to load font: " + path);
		}
	}

	const sf::Font* ResourceSystem::GetFontShared(const std::string& name) const
	{
		auto it = fonts.find(name);
		if (it != fonts.end())
		{
			return it->second;
		}

		LOG_ERROR("Font '" + name + "' not found in ResourceSystem");
		throw std::runtime_error("Font not found: " + name);

		// дефолтный шрифт
		if (fonts.find("default") != fonts.end())
		{
			return fonts.at("default");
		}
		return nullptr;
	}

	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = texturesMaps.find(name);
		auto textures = textureMap->second;
		return textures[elementIndex];
	}

	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMap = texturesMaps.find(name);
		auto textures = textureMap->second;
		return new sf::Texture(*textures[elementIndex]);
	}

	int ResourceSystem::GetTextureMapElementCount(const std::string& name) const
	{
		auto textureMap = texturesMaps.find(name);
		auto textures = textureMap->second;
		return textures.size();
	}

	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = texturesMaps.find(name);
		auto deletingtextures = textureMap->second;

		for (int i = 0; i < deletingtextures.size(); i++)
		{
			delete deletingtextures[i];
		}
		texturesMaps.erase(textureMap);
	}

	void ResourceSystem::clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllFonts();
	}

	void ResourceSystem::DeleteFont(const std::string& name)
	{
		auto it = fonts.find(name);
		if (it != fonts.end())
		{
			delete it->second;
			fonts.erase(it);
			LOG_INFO("Font '" + name + "' deleted");
		}
		else
		{
			LOG_WARN("Attempt to delete non-existing font: '" + name + "'");
		}
	}

	void ResourceSystem::DeleteAllFonts()
	{
		for (auto& pair : fonts)
		{
			delete pair.second;
		}
		fonts.clear();
		LOG_INFO("All fonts deleted");
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}

	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : textures)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}

} // namespace Engine
