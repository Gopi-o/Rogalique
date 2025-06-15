#pragma once
#include "Component.h"
#include "SoundComponent.h"
#include <unordered_map>
#include <string>


namespace Engine
{
	class SoundManagerComponent : public Component
	{
	public:
		SoundManagerComponent(GameObject* gameObject);

		void Update(float deltaTime) override {}
		void Render() override {}

		void AddSound(const std::string& soundName, const std::string& filePath, float volume = 100.f, bool isLoop = false);
		void PlaySound(const std::string& soundName);
		void StopSound(const std::string& soundName);
		void SetVolume(const std::string& soundName, float volume);

	private:
		std::unordered_map<std::string, SoundComponent*> sounds;

	};
}

