#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "Platform.h"
#include "Ball.h"
#include "Bonus.h"
#include "ICommand.h"
#include "LevelLoader.h"
#include "BlockFactory.h"
#include "ScoreManager.h"
#include "Memento.h"
#include "IObserver.h"

#include <unordered_map>

namespace ArkanoidGame
{
	class Game;
	class Block;
	class BlockFactory;

	class GameStatePlayingData : public GameStateData, public IObserver, public std::enable_shared_from_this<GameStatePlayingData>
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;
		void LoadNextLevel();
		void Notify(std::shared_ptr<IObservable> observable) override;
		void CreateBonus(const sf::Vector2f& position); 
		void UpdateBonuses(float timeDelta); 
		void HandleBonusCollisions();
		int GetScore() const { return highScore; }

		void ActivateFragileBlocks(float duration);
		void DeactivateFragileBlocks();



	private:
		void createBlocks();
		void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX,
			bool& needInverseDirY);

		Memento CreateMemento() const;
		void RestoreFromMemento(const Memento& memento);

		void SaveGame(const std::string& filename);
		void LoadGame(const std::string& filename);

		// Resources
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::Font font;
		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer gameOverSoundBuffer;

		// Game data
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::shared_ptr<Block>> blocks;
		std::vector<std::shared_ptr<Bonus>> bonuses;
		std::shared_ptr<ScoreManager> scoreManager;


		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;

		// Sounds
		sf::Sound gameOverSound;

		//Blocks creator
		std::unordered_map<BlockType, std::unique_ptr<BlockFactory>> factories;
		int unbreackableBlocksCount = 0;

		//Levels
		LevelLoader levelLoder;
		int currentLevel = 0;

		int score = 0;
		int highScore = 0;

		//BlockCommand
		std::shared_ptr<ICommand> fragileBlockCommand; 
		bool fragileBlocksActive = false; 
		float fragileDuration = 0.0f;

	};
}
