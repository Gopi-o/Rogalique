#include "pch.h"
#include "Component.h"
#include <iostream>


namespace Engine {

	Component::Component(GameObject* gameObject) : gameObject(gameObject) {}

	Component::~Component()
	{
		std::cout << "Deleted component: " << this << std::endl;
	}



	GameObject* Component::GetGameObject()
	{
		return gameObject;
	}


}
