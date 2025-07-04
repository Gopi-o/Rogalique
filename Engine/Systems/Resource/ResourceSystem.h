#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFMl/Audio.hpp>

namespace Engine
{
	class ResourceSystem
	{
	public:
		static ResourceSystem* Instance();

		void LoadTexture(const std::string& name, std::string sourcePath, bool isSwooth = true);
		const sf::Texture* GetTextureShared(const std::string& name) const;
		sf::Texture* GetTextureCopy(const std::string& name) const;
		void DeleteSharedTexture(const std::string& name);

		void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);

		void LoadFont(const std::string& name, const std::string& path);
		const sf::Font* GetFontShared(const std::string& name) const;

		const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
		sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
		int GetTextureMapElementCount(const std::string& name) const;
		void DeleteSharedTextureMap(const std::string& name);
		void clear();
		void DeleteFont(const std::string& name);
		void DeleteAllFonts();

	private:
		std::map<std::string, sf::Font*> fonts;
		std::map<std::string, sf::Texture*> textures;
		std::map<std::string, std::vector<sf::Texture*>> texturesMaps;

		ResourceSystem() {}
		~ResourceSystem() {}

		ResourceSystem(ResourceSystem const&) = delete;
		ResourceSystem& operator=(ResourceSystem const&) = delete;

		void DeleteAllTextures();
		void DeleteAllTextureMaps();
	};
} // namespace Engine
