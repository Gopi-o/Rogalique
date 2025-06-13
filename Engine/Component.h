#pragma once

namespace Engine {
	class GameObject;
	
	class Component
	{
	public:
		Component(GameObject* gameObject);
		virtual~Component();

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		GameObject* GetGameObject();
		
	protected:
		GameObject* gameObject;
	};

}

