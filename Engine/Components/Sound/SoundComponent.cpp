#include "pch.h"
#include "SoundComponent.h"

namespace Engine
{
	SoundComponent::SoundComponent(GameObject* gameObject) : Component(gameObject)
	{
		m_soundBuffer = std::make_unique<sf::SoundBuffer>();
		m_sound = std::make_unique<sf::Sound>();
	}

	SoundComponent::~SoundComponent()
	{
		if (m_sound)
		{	
			m_sound->stop();
		}
	}

	void SoundComponent::Update(float deltaTime)
	{
	}

	void SoundComponent::Render()
	{
	}

	bool SoundComponent::LoadFromFile(const sf::String& filename)
	{
		if (!m_soundBuffer->loadFromFile(filename))
		{
			return false;
		}
		m_sound->setBuffer(*m_soundBuffer);
		return true;
	}

	void SoundComponent::Play()
	{
		m_sound->play();
	}

	void SoundComponent::Stop()
	{
		m_sound->stop();
	}

	void SoundComponent::Pause()
	{
		m_sound->pause();
	}

	void SoundComponent::SetLoop(bool loop)
	{
		m_sound->setLoop(loop);
	}

	void SoundComponent::SetVolume(float value)
	{
		m_sound->setVolume(value);
	}

	sf::SoundSource::Status SoundComponent::GetStatus() const
	{
		return m_sound->getStatus();
	}







}