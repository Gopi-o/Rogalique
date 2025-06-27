#pragma once

namespace Engine
{
	class Scene
	{
	public:
		virtual void Start() = 0;
		virtual void Restart() = 0;
		virtual void Stop() = 0;
		virtual void Update(float deltaTime) = 0;
	};
}

