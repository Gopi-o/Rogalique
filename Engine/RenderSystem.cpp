#include "pch.h"
#include "RenderSystem.h"

namespace Engine 
{
    RenderSystem* Engine::RenderSystem::Instance()
    {
        static RenderSystem render;
        return &render;
    }

    void RenderSystem::SetMainWindow(sf::RenderWindow* newWindow)
    {
        if (window != nullptr)
        {
            delete window;  
        }
        window = newWindow;
    }

    sf::RenderWindow& RenderSystem::GetMainWindow() const
    {
        if (window == nullptr)
        {
            throw std::runtime_error("Window is not initialized!");
        }
        return *window;
    }

    void RenderSystem::Render(const sf::Drawable& drawable)
    {
        if (window == nullptr)
        {
            throw std::runtime_error("Cannot render: window is not initialized!");
        }
        window->draw(drawable);
    }

    
}
