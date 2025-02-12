#pragma once

namespace ArkanoidGame
{
	class Memento 
	{
	public:
		Memento(int score, int currentLevel)
			: score(score), level(currentLevel) {}

		int getScore() const { return score; }
		int GetLevel() const { return level; }


	private:
		int score;
		int level;
	};
}