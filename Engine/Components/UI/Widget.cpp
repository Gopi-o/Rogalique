#include "pch.h"
#include "Widget.h"
#include <ResourceSystem/Sprite.h>

namespace Engine
{
	Widget::Widget(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	Widget::~Widget()
	{
		for (auto* Child : children)
		{
			delete Child;
		}
	}

	void Widget::Update(float deltaTime)
	{
		for (auto* child : children)
		{
			if (child && child->IsVisible())
				child->Update(deltaTime);
		}
	}

	void Widget::Render()
	{
		if (!IsVisible())
			return;

		if (useSprite)
			RenderSystem::Instance()->Render(sprite);

		for (auto* child : children)
		{
			if (child)
				child->Render();
		}
	}

	void Widget::RenderWidgetOnly()
	{
		if (!IsVisible())
			return;

		if (useSprite)
			RenderSystem::Instance()->Render(sprite);
	}

	void Widget::HandleEvent(const sf::Event& event)
	{
		if (!IsVisible())
			return;

		for (auto it = children.rbegin(); it != children.rend(); ++it)
		{
			if (*it)
			{
				(*it)->HandleEvent(event);
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			const Vector2Df mousePos(event.mouseMove.x, event.mouseMove.y);
			const bool nowHovered = IsPointInside(mousePos);

			if (nowHovered && !isHovered)
			{
				isHovered = true;
				if (OnHovered)
					OnHovered();
			}
			else if (!nowHovered && isHovered)
			{
				isHovered = false;
				if (OnUnhovered)
					OnUnhovered();
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed && isHovered)
		{
			isPressed = true;
			if (OnPressed)
				OnPressed();
		}
		else if (event.type == sf::Event::MouseButtonReleased && isPressed)
		{
			isPressed = true;
			if (OnReleased)
				OnReleased();
		}
	}

	void Widget::AddChild(Widget* child)
	{
		if (child && child != this)
		{
			children.push_back(child);
			child->parent = this;
			child->UpdateTransform();
		}
	}

	void Widget::RemoveChild(Widget* child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
		{
			(*it)->parent = nullptr;
			children.erase(it);
		}
	}

	Vector2Df Widget::CalculateDesiredSize() const
	{
		if (useSprite)
		{
			auto bounds = sprite.getGlobalBounds();
			return { bounds.width, bounds.height };
		}
		return size;
	}

	void Widget::SetPosition(const Vector2Df& newPosition)
	{
		if (position != newPosition)
		{
			position = newPosition;
			UpdateTransform();
		}
	}

	void Widget::SetSize(const Vector2Df& newSize)
	{
		if (size != newSize)
		{
			size = newSize;
			if (useSprite)
			{
				SetSpriteSize(sprite, size.x, size.y);
			}
			UpdateTransform();
		}
	}

	void Widget::SetVisibility(EWidgetVisibility newVisibility)
	{
		if (visibility != newVisibility)
		{
			visibility = newVisibility;
		}
	}

	bool Widget::IsVisible() const
	{
		return visibility == EWidgetVisibility::Visible && (parent == nullptr || parent->IsVisible());
	}

	bool Widget::IsPointInside(const Vector2Df& point) const
	{
		if (useSprite)
		{
			return sprite.getGlobalBounds().contains(sf::Vector2f(point.x, point.y));
		}
		return point.x >= position.x && point.y >= position.y && point.x <= position.x + size.x && point.y <= position.y + size.y;
	}

	void Widget::UpdateTransform()
	{
		if (useSprite)
		{
			sprite.setPosition(sf::Vector2f(position.x, position.y));
		}

		UpdateChildrenTransforms();
	}

	void Widget::UpdateChildrenTransforms()
	{
		for (auto* child : children)
		{
			if (child)
			{
				child->UpdateTransform();
			}
		}
	}

} // namespace Engine
