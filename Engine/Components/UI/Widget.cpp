#include "pch.h"
#include "Widget.h"
#include <ResourceSystem/Sprite.h>
#include "Layout.h"
#include <Systems/Logger.h>

namespace Engine
{
	Widget::Widget(GameObject* gameObject)
		: Component(gameObject)
	{
		OnHovered = []() {};
		OnUnhovered = []() {};
		OnPressed = []() {};
		OnReleased = []() {};
	}

	Widget::~Widget()
	{
		/*for (auto* Child : children)
		{
			delete Child;
		}*/
		MarkInvalid();
		for (auto* child : children)
		{
			if (child)
				child->parent = nullptr;
		}
		children.clear();
	}

	void Widget::Update(float deltaTime)
	{
		for (auto* child : children)
		{
			if (child == nullptr && child->IsVisible() && child->GetParent() == nullptr)
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

		// Создаем временную копию для безопасной итерации
		const auto tempChildren = children;
		bool eventHandled = false;

		// Обратная итерация по временной копии
		for (auto it = tempChildren.rbegin(); it != tempChildren.rend(); ++it)
		{
			Widget* child = *it;
			if (child == nullptr || child->GetParent() != this)
				continue;

			child->HandleEvent(event);
			if (child->GetIsPressed())
			{
				eventHandled = true;
				break;
			}
		}

		if (eventHandled)
		{
			if (isHovered)
			{
				isHovered = false;
				try
				{
					if (OnUnhovered)
					{
						if (this->gameObject == nullptr)
							OnUnhovered();
					}
				}
				catch (...)
				{
				}
			}
			return;
		}

		// Обработка событий для текущего виджета
		try
		{
			if (this->GetParent() == nullptr)
			{
				return;
			}
			const Vector2Df mousePos = GetMousePosition(event);
			const bool nowHovered = IsPointInside(mousePos);

			if (event.type == sf::Event::MouseMoved)
			{
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
			else if (event.type == sf::Event::MouseButtonPressed && nowHovered)
			{
				isPressed = true;
				if (OnPressed)
					OnPressed();
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (isPressed)
				{
					isPressed = false;
					if (nowHovered && OnReleased)
						OnReleased();
				}
			}
		}
		catch (...)
		{
			// Логирование ошибки
		}
	}

	void Widget::AddChild(Widget* child)
	{

		if (child == nullptr || child == this || !child->IsValid())
			return;
		children.push_back(child);
		child->parent = this;
		child->UpdateTransform();
	}

	void Widget::RemoveChild(Widget* child)
	{
		if (child == nullptr)
			return;

		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
		{
			(*it)->parent = nullptr;
			children.erase(it);
		}
	}

	Vector2Df Widget::GetMousePosition(const sf::Event& event) const
	{
		if (event.type == sf::Event::MouseMoved)
			return { static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y) };
		else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
			return { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
		return { 0.0f, 0.0f };
	}

	Vector2Df Widget::GetAbsolutePosition() const
	{
		Vector2Df result = position;
		if (parent)
		{
			result = result + parent->GetAbsolutePosition();
			// Добавляем padding родителя, если это Layout
			if (auto layout = dynamic_cast<Layout*>(parent))
			{
				result = result + layout->GetPadding();
			}
		}
		return result;
	}

	Vector2Df Widget::GetRelativePosition() const
	{
		return position;
	}

	Vector2Df Widget::CalculateDesiredSize() const
	{
		// 1. Если задан явный размер - используем его
		if (size.x > 0 && size.y > 0)
			return size;

		// 2. Если используется спрайт - возвращаем его локальные границы (не глобальные!)
		if (useSprite && sprite.getTexture())
		{
			const sf::FloatRect bounds = sprite.getLocalBounds();
			return { bounds.width, bounds.height };
		}

		// 3. Возвращаем минимальный допустимый размер
		return { 10.0f, 10.0f };
	}

	void Widget::SetPosition(const Vector2Df& newPosition)
	{
		if (position != newPosition)
		{
			// Вычисляем смещение для всех дочерних элементов
			Vector2Df offset = newPosition - position;

			// Обновляем позицию текущего виджета
			position = newPosition;

			if (useSprite)
			{
				sprite.setPosition(position.x, position.y);
			}

			// Обновляем позиции всех дочерних элементов с учетом смещения
			UpdateChildrenTransforms();
		}
	}

	void Widget::SetRelativePosition(const Vector2Df& relativePos)
	{
		position = relativePos;
		UpdateTransform();
	}

	void Widget::SetSize(const Vector2Df& newSize)
	{
		if (size == newSize || isUpdatingTransform)
			return;

		isUpdatingTransform = true;
		size = newSize;

		if (useSprite)
		{
			SetSpriteSize(sprite, size.x, size.y);
		}

		UpdateChildrenTransforms();
		isUpdatingTransform = false;
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
		if (this == nullptr)
		{
			LOG_ERROR("Invalid Widget pointer in IsVisible()");
			return false;
		}
		return visibility == EWidgetVisibility::Visible;
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
		if (isUpdatingTransform)
			return; // Защита от рекурсии

		isUpdatingTransform = true;

		Vector2Df finalPosition = GetAbsolutePosition();
		if (useSprite)
		{
			sprite.setPosition(finalPosition.x, finalPosition.y);
		}

		UpdateChildrenTransforms();

		isUpdatingTransform = false;
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
