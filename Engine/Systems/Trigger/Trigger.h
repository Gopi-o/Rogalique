#pragma once

namespace Engine
{
	class ColliderComponent;
	struct Trigger
	{
	public:
		Trigger(ColliderComponent* newFirst, ColliderComponent* newSecond)
			: first(newFirst), second(newSecond) {
			};
		ColliderComponent* GetFirst() const { return first; }
		ColliderComponent* GetSecond() const { return second; }

	private:
		ColliderComponent* first;
		ColliderComponent* second;
	};
} // namespace Engine