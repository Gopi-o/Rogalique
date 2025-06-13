#pragma once
#include "Component.h"
#include <SFML/Audio.hpp>
#include <memory>

namespace Engine
{
	class SoundComponent : public Component
	{
	public:
		SoundComponent(GameObject* gameObject);
		~SoundComponent();

		void Update(float deltaTime) override;
		void Render() override;

		bool LoadFromFile(const sf::String& filename);

		void Play();
		void Stop();
		void Pause();
		void SetLoop(bool loop);
		void SetVolume(float value);
		sf::SoundSource::Status GetStatus() const;

	protected:
		std::unique_ptr<sf::SoundBuffer> m_soundBuffer;
		std::unique_ptr<sf::Sound> m_sound;
	};

}

