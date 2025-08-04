#pragma once
#include <Core/Scene.h>
#include <Components/UI/Button.h>
#include <Components/UI/Canvas.h>
#include <Components/UI/Image.h>
#include <Components/UI/Layout.h>
#include <Components/UI/TextBlock.h>
#include <Components/UI/Widget.h>
#include <SFML/Window/Event.hpp>

using namespace Engine;

namespace RogaliqueGame
{
	class MainMenuScene : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
		void Update(float deltaTime) override;
		void HandleEvent(const sf::Event& event) override;

	private:
		void CreateButton(const std::string& text, std::function<void()> onClick);
		void OnNewGameClicked();
		void OnLoadGameClicked();
		void OnOptionsClicked();
		void OnExitClicked();

		Canvas* canvas = nullptr;
		Image* background = nullptr;
		VerticalBox* mainContainer = nullptr;
		TextBlock* titleText = nullptr;
		std::vector<Button*> menuButtons;
	};
};
