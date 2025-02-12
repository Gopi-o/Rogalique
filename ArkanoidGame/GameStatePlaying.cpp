#include "GameStatePlaying.h"
#include "Application.h"
#include "Block.h"
#include "Game.h"
#include "Text.h"
#include "fstream"
#include "FastFireBallStrategy.h"
#include "FragileBlockCommand.h"
#include "BonusManager.h"
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
	void GameStatePlayingData::Init()
	{
		// Init game resources (terminate if error)
		assert(font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"));
		assert(gameOverSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "Death.wav"));

		//factoriesInit
		factories.emplace(BlockType::Simple, std::make_unique<SimpleBlockFactory>());
		factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
		factories.emplace(BlockType::Unbreackable, std::make_unique<UnbreackableBlockFactory>());
		factories.emplace(BlockType::Glass, std::make_unique<GlassBlockFactory>());

		// Init background
		background.setSize(sf::Vector2f(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 0, 0));

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, ESC to pause");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));


		scoreManager = std::make_shared<ScoreManager>();
		LoadGame(SETTINGS.SAVE_GAME_CONFIG_PATH);
		scoreManager->AddObserver(weak_from_this());

		gameObjects.emplace_back(std::make_shared<Platform>(sf::Vector2f({ SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT / 2.f })));

		auto ball = std::make_shared<Ball>(sf::Vector2f({ SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT - SETTINGS.BALL_SIZE / 2.f }));
		ball->AddObserver(weak_from_this());
		gameObjects.emplace_back(ball);


		createBlocks();

		// Init sounds
		gameOverSound.setBuffer(gameOverSoundBuffer);
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PauseGame();
			}
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{
		static auto updateFunctor = [timeDelta](auto obj) { obj->Update(timeDelta); };

		std::for_each(gameObjects.begin(), gameObjects.end(), updateFunctor);
		std::for_each(blocks.begin(), blocks.end(), updateFunctor);

		BonusManager::Update(timeDelta);
		UpdateBonuses(timeDelta);
		HandleBonusCollisions();

		std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
		std::shared_ptr <Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);

		auto isCollision = platform->CheckCollision(ball);

		bool needInverseDirX = false;
		bool needInverseDirY = false;


		bool hasBrokeOneBlock = false;
		//remove-erase idiom
		blocks.erase(
			std::remove_if(blocks.begin(), blocks.end(),
				[ball, &hasBrokeOneBlock, &needInverseDirX, &needInverseDirY, this](auto block) {
					if ((!hasBrokeOneBlock) && block->CheckCollision(ball)) {
						if (dynamic_cast<GlassDestroyableBlock*>(block.get())) {
							hasBrokeOneBlock = true;
							return block->IsBroken();
						}
						else {
							hasBrokeOneBlock = true;
							const auto ballPos = ball->GetPosition();
							const auto blockRect = block->GetRect();

							GetBallInverse(ballPos, blockRect, needInverseDirX, needInverseDirY);
						}
					}
					return block->IsBroken();
				}),
			blocks.end()
		);
		//if (fragileBlocksActive) {
		//	for (auto& block : blocks) {
		//		if (block->GetCollision(ball)) {
		//			fragileBlockCommand->Execute(*block); // Выполняем команду для блока
		//		}
		//	}

		//	// Уменьшаем оставшееся время действия бонуса
		//	fragileDuration -= timeDelta;
		//	if (fragileDuration <= 0) {
		//		DeactivateFragileBlocks(); // Деактивируем позже
		//	}
		//}
		if (needInverseDirX) {
			ball->InvertDirectionX();
		}
		if (needInverseDirY) {
			ball->InvertDirectionY();
		}
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);

		static auto drawFunc = [&window](auto block) { block->Draw(window); };
		// Draw game objects
		std::for_each(gameObjects.begin(), gameObjects.end(), drawFunc);
		std::for_each(blocks.begin(), blocks.end(), drawFunc);
		std::for_each(bonuses.begin(), bonuses.end(), drawFunc);

		
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}

	void GameStatePlayingData::LoadNextLevel()
	{
		if (currentLevel >= levelLoder.GetLevelCount() - 1) {
			Game& game = Application::Instance().GetGame();

			game.WinGame();
			
		}
		else
		{
			std::shared_ptr <Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
			std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
			platform->restart();
			ball->restart();

			blocks.clear();
			++currentLevel;
			createBlocks();
		}
	}

	void GameStatePlayingData::createBlocks()
	{
		for (const auto& pair : factories)
		{
			pair.second->ClearCounter();
		}
		auto self = weak_from_this();

		auto level = levelLoder.GetLevel(currentLevel);

		for (auto pairPosBlockTYpe : level.m_blocks)
		{
			auto blockType = pairPosBlockTYpe.second;
			sf::Vector2i pos = pairPosBlockTYpe.first;

			sf::Vector2f position{
				(float)(SETTINGS.BLOCK_SHIFT + SETTINGS.BLOCK_WIDTH / 2.f + pos.x * (SETTINGS.BLOCK_WIDTH + SETTINGS.BLOCK_SHIFT))
				, (float)pos.y * SETTINGS.BLOCK_HEIGHT
			};



			blocks.emplace_back(factories.at(blockType)->CreateBlock(position));
			blocks.back()->AddObserver(self);
		}


		for (const auto& pair : factories)
		{
			unbreackableBlocksCount += pair.second->GetcreatedBreackableBlocksCount();
		}
	}

	void GameStatePlayingData::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect, bool& needInverseDirX, bool& needInverseDirY) {

		if (ballPos.y > blockRect.top + blockRect.height)
		{
			needInverseDirY = true;
		}
		if (ballPos.x < blockRect.left)
		{
			needInverseDirX = true;
		}
		if (ballPos.x > blockRect.left + blockRect.width)
		{
			needInverseDirX = true;
		}
	}

	Memento GameStatePlayingData::CreateMemento() const
	{
		return Memento(highScore, currentLevel);
	}

	void GameStatePlayingData::RestoreFromMemento(const Memento& memento)
	{
		highScore = memento.getScore();
		currentLevel = memento.GetLevel();

	}

	void GameStatePlayingData::SaveGame(const std::string& filename)
	{
		Memento memento = CreateMemento();
		std::ofstream ofs(filename);
		if (ofs.is_open()) {
			ofs << memento.getScore() << " " << memento.GetLevel() << std::endl;
			ofs.close();
		}
	}

	void GameStatePlayingData::LoadGame(const std::string& filename)
	{
		std::ifstream ifs(filename);
		if (ifs.is_open()) {
			int fileScore, fileCurrentlevel;
			ifs >> fileScore >> fileCurrentlevel;
			RestoreFromMemento(Memento(fileScore, fileCurrentlevel));
		}
	}




	void GameStatePlayingData::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto block = std::dynamic_pointer_cast<Block>(observable); block) {
			--unbreackableBlocksCount;

			
			CreateBonus(block->GetPosition());

			if (dynamic_cast<StrongDestroyableBlock*>(block.get())) {
				scoreManager->AddScore(SETTINGS.STRONG_BLOCK_POINTS);
			}
			else if (dynamic_cast<GlassDestroyableBlock*>(block.get())) {
				scoreManager->AddScore(SETTINGS.GLASS_BLOCK_POINTS);
			}
			else if (dynamic_cast<SmoothDestroyableBlock*>(block.get())) {
				scoreManager->AddScore(SETTINGS.SAMPLE_BLOCK_POINTS);
			}
			

			Game& game = Application::Instance().GetGame();
			if (unbreackableBlocksCount == 0) {
				game.LoadNextLevel();
			}
		}
		else if (auto ball = std::dynamic_pointer_cast<Ball>(observable); ball)
		{
			if (ball->GetPosition().y > gameObjects.front()->GetRect().top) {
				score = scoreManager->GetTotalScore();
				
				if (score >= highScore) {
					highScore = score;
				}
				Application::Instance().GetGame().SetScore(highScore);
				SaveGame(SETTINGS.SAVE_GAME_CONFIG_PATH);
				gameOverSound.play();
				Application::Instance().GetGame().LooseGame();
			}
		}
	}

	void GameStatePlayingData::CreateBonus(const sf::Vector2f& position)
	{
		if (rand() % 100 < 80)
		{

			BonusType type = static_cast<BonusType>(rand() % 5); 
			sf::Color color; 

			switch (type)
			{
			case BonusType::FireBall:
				color = sf::Color::Red; 
				break;
			case BonusType::FragileBlocks:
				color = sf::Color::Blue; 
				break;
			case BonusType::IncreasePlatformSize:
				color = sf::Color::Green; 
				break;
			case BonusType::IncreasePlatformSpeed:
				color = sf::Color::Cyan; 
				break;
			case BonusType::BounceBallRandomDirection:
				color = sf::Color::Magenta;
				break;
			default:
				color = sf::Color::White; 
				break;
			}

			sf::Vector2f bonusPosition = position;
			bonusPosition.y += SETTINGS.BLOCK_HEIGHT;

			auto bonus = std::make_shared<Bonus>(bonusPosition, color, type);
			bonuses.push_back(bonus);
		}
	}

	void GameStatePlayingData::UpdateBonuses(float timeDelta)
	{
		for (auto& bonus : bonuses)
		{
			bonus->Update(timeDelta);
		}

		// Удаляем неактивные бонусы
		bonuses.erase(
			std::remove_if(bonuses.begin(), bonuses.end(),
				[](const std::shared_ptr<Bonus>& bonus) {
					return !bonus->IsActive();
				}),
			bonuses.end()
		);
	}

	void GameStatePlayingData::HandleBonusCollisions()
	{
		auto platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		auto ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
		if (!platform)
		{
			return;
		}

		for (auto& bonus : bonuses)
		{
			if (bonus->CheckCollision(platform))
			{
				bonus->OnHit(); // Деактивируем бонус
				// Активируем эффект бонуса (в зависимости от типа)
				switch (bonus->GetType())
				{
				case BonusType::FireBall: // Новый бонус
					
					BonusManager::ActivateFastBallBonus(5.0f);
					break;
				case BonusType::FragileBlocks:
					BonusManager::ActivateFragileBonus(10.0f); 
					break;
				case BonusType::IncreasePlatformSize:
					BonusManager::ActivateSizeBonus(10.0f);
					break;
				case BonusType::IncreasePlatformSpeed:
					BonusManager::ActivateSpeedBonus(10.0f);
					break;
				case BonusType::BounceBallRandomDirection:
					BonusManager::ActivateBounceDirectionBonus(10.0f);
					break;
				}
			}
		}
	}

}
