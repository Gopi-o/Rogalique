#include "pch.h"
#include "GameObject.h"

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
			component->Update(deltaTime);
		}
	}
	void GameObject::Render()
	{
		for (auto& component : components)
		{
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
}