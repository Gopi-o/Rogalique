#include "GameStatePlaying.h"
#include "Application.h"

#include "Game.h"
#include "Text.h"
#include "fstream"
#include <assert.h>
#include <sstream>

namespace RogaliqueGame
{
	void GameStatePlayingData::Init()
	{
		// Init game resources (terminate if error)
		assert(font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"));
		assert(gameOverSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "Death.wav"));


		// Init background
		background.setSize(sf::Vector2f(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 0, 0));

		PlayText.setFont(font);
		PlayText.setCharacterSize(60);
		PlayText.setFillColor(sf::Color::Yellow);
		PlayText.setString("To be done...");
		

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use P key to Pouse, ESC to end");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));
		// Init sounds
		gameOverSound.setBuffer(gameOverSoundBuffer);
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				gameOverSound.play();
				Application::Instance().GetGame().LooseGame();
			}
			else if (event.key.code == sf::Keyboard::P)
			{
				Application::Instance().GetGame().PauseGame();
			}
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{

	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);


	

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);

		PlayText.setOrigin(GetTextOrigin(inputHintText, { 0.5f, 0.5f }));
		PlayText.setPosition((SETTINGS.SCREEN_WIDTH / 2)-55.f, SETTINGS.SCREEN_HEIGHT/ 2);
		window.draw(PlayText);
	}


}
