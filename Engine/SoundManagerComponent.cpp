#include "pch.h"
#include "SoundManagerComponent.h"
#include "GameObject.h"
#include "Logger.h"


namespace Engine
{
	SoundManagerComponent::SoundManagerComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void SoundManagerComponent::AddSound(const std::string& soundName, const std::string& filePath, float volume, bool isLoop)
	{
		LOG_INFO("Attempting to add sound: " + soundName + " from path: " + filePath);

		auto soundComponent = gameObject->AddComponent<SoundComponent>();
		if (!soundComponent) {
			LOG_ERROR("Failed to create SoundComponent");
			return;
		}

		if (soundComponent->LoadFromFile(filePath)) {
			LOG_INFO("Sound file loaded successfully");
			soundComponent->SetVolume(volume);
			soundComponent->SetLoop(isLoop);
			sounds[soundName] = soundComponent;
			LOG_INFO("Sound added to map: " + soundName);

			// ѕроверим, что звук действительно добавилс€ в карту
			if (sounds.find(soundName) != sounds.end()) {
				LOG_INFO("Sound verified in map: " + soundName);
			}
			else {
				LOG_ERROR("Sound not found in map after adding: " + soundName);
			}
		}
		else {
			LOG_ERROR("Failed to load sound file: " + filePath);
		}
	}

	void SoundManagerComponent::PlaySound(const std::string& soundName)
	{
		LOG_INFO("PlaySound called for: " + soundName);

		auto it = sounds.find(soundName);
		if (it != sounds.end()) {
			LOG_INFO("Sound found in map, attempting to play");
			it->second->Stop();
			it->second->Play();
			LOG_INFO("Play command sent to sound component");
		}
		else {
			LOG_ERROR("Sound not found in map: " + soundName);
			std::string availableSounds = "Available sounds: ";
			for (const auto& sound : sounds) {
				availableSounds += sound.first + ", ";
			}
			LOG_INFO(availableSounds);
		}
	}

	void SoundManagerComponent::StopSound(const std::string& soundName)
	{
		auto it = sounds.find(soundName);
		if (it != sounds.end())
		{
			it->second->Stop();
		}
	}

	void SoundManagerComponent::SetVolume(const std::string& soundName, float volume)
	{
		auto it = sounds.find(soundName);
		if (it != sounds.end())
		{
			it->second->SetVolume(volume);
		}
	}
}