#include "pch.h"
#include "GameObject.h"
#include <Systems/Logger.h>

namespace Engine
{
	GameObject::GameObject()
	{
		name = "GameObject";
		tag = "Untagged";
		AddComponent<TransformComponent>();
	}
	GameObject::GameObject(std::string newName)
	{
		name = newName;
		tag = "Untagged";
		AddComponent<TransformComponent>();
	}

	GameObject::~GameObject()
	{
		for (auto component : components)
		{
			delete component;
		}
		components.clear();
		children.clear();
	}

	std::string GameObject::GetName() const
	{
		return name;
	}

	void GameObject::Print(int depth) const
	{
		std::cout << std::string(depth * 2, ' ') << GetName() << std::endl;
		for (auto& component : components)
		{
			std::cout << std::string(depth * 2, ' ') << "::" << component << std::endl;
		}

		for (GameObject* child : children)
		{
			child->Print(depth + 1);
		}
	}

	std::string GameObject::GetTag() const
	{
		return tag;
	}

	void GameObject::SetTag(const std::string& newTag)
	{
		tag = newTag;
	}

	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			if (component)
			{
				component->Update(deltaTime);
			}
			else
				LOG_ERROR("Null component found in update component in GameObject!");
		}
	}
	void GameObject::Render()
	{
		for (auto& component : components)
		{
			if (!shouldRender)
				return;
			component->Render();
		}
	}

	void GameObject::AddChild(GameObject* child)
	{
		children.push_back(child);
	}
	void GameObject::RemoveChild(GameObject* child)
	{
		children.erase(std::remove_if(children.begin(), children.end(), [child](GameObject* obj) { return obj == child; }), children.end());
	}
} // namespace Engine