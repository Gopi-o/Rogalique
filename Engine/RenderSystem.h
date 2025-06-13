#pragma once
#include <SFML/Graphics.hpp>

namespace Engine
{
	class RenderSystem
	{
	public:
		static RenderSystem* Instance();

		void SetMainWindow(sf::RenderWindow* newWindow);
		sf::RenderWindow& GetMainWindow() const;

		void Render(const sf::Drawable& drawable);

	private:
		sf::RenderWindow* window;

		RenderSystem() {}
		~RenderSystem() {}

		RenderSystem(RenderSystem const&) = delete;
		RenderSystem& operator= (RenderSystem const&) = delete;
	};
}

