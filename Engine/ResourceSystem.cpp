#include "pch.h"
#include "ResourceSystem.h"


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

}
