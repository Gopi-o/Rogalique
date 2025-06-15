#pragma once
#include "Component.h"
#include <unordered_map>
#include "string"
#include <variant>
#include <functional>
#include <any>

namespace Engine
{
	class StatsComponent : public Component
	{
	public:
		StatsComponent(GameObject* gameObject);
		virtual ~StatsComponent() = default;

		void Update(float deltaTime) override {};
		void Render() override {};

		template<typename T>
		void SetValue(const std::string& key, const T& value) {
			values[key] = std::any(value);
			if (onValueChanged) {
				onValueChanged(key, values[key]);
			}
		}

		template<typename T>
		T GetValue(const std::string& key) const {
			auto it = values.find(key);
			if (it != values.end())
			{
				return std::any_cast<T>(it->second);
			}
			return T();
		}

		template<typename T>
		bool HasValue(const std::string& key) const {
			auto it = values.find(key);
			return it != values.end() && it->second.type() == typeid(T);
		}


		using ValueChangedCallback = std::function<void(const std::string&, const std::any&)>;
		void SetOnValueChangedCallback(ValueChangedCallback callback) {
			onValueChanged = callback;
		}

	private:
		std::unordered_map<std::string, std::any> values;
		ValueChangedCallback onValueChanged;
	};
}

