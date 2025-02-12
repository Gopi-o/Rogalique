#pragma once
#include "IObserver.h"



namespace ArkanoidGame
{
	class ScoreManager : public IObservable
	{
	private:
		int totalScore;

	public:
		ScoreManager() : totalScore(0) {}

		void AddScore(int score) {
			totalScore += score;
			Emit();
		}

		int GetTotalScore() const {
			return totalScore;
		}
	};

}

