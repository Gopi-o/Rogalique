#pragma once
#include "EventsTemp.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace Engine
{

	class EventSystem
	{
	public:
		using EventCallback = std::function<void(const EventsTemp&)>;

		static EventSystem& GetInstance()
		{
			static EventSystem instance;
			return instance;
		}

		void Subscribe(const std::string& eventType, EventCallback callback)
		{
			listeners[eventType].push_back(callback);
		}

		void Unsubscribe(const std::string& eventType, EventCallback callback)
		{
			auto& callbacks = listeners[eventType];
			callbacks.erase(
				std::remove_if(callbacks.begin(), callbacks.end(),
					[&callback](const EventCallback& cb) {
						return cb.target_type() == callback.target_type();
					}),
				callbacks.end());
		}

		void Dispatch(const EventsTemp& event)
		{
			const std::string& type = event.GetType();
			if (listeners.find(type) != listeners.end())
			{
				for (const auto& callback : listeners[type])
				{
					callback(event);
				}
			}
		}

	private:
		EventSystem() = default;
		std::unordered_map<std::string, std::vector<EventCallback>> listeners;
	};
} // namespace Engine
