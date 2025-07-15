#pragma once
#include <Components/Component.h>
#include <SFML/Graphics.hpp>
#include <Math/Vector.h>
#include <functional>
#include <Systems/Render/RenderSystem.h>

namespace Engine
{
	enum class EWidgetVisibility
	{
		Visible,  // Видим и интерактивен
		Hidden,	  // Невидим, но сохраняет место в layout
		Collapsed // Невидим и не занимает место
	};

	class Widget : public Component
	{
	public:
		Widget(GameObject* gameObject);
		virtual ~Widget();

		//=== Основные методы ===//
		void Update(float deltaTime) override;
		void Render() override;
		void RenderWidgetOnly();
		bool IsInCanvas() const { return isInCanvas; }
		virtual void HandleEvent(const sf::Event& event);

		//=== Иерархия ===//
		virtual void AddChild(Widget* child);
		virtual void RemoveChild(Widget* child);
		Widget* GetParent() const { return parent; }
		const std::vector<Widget*>& GetChildren() const { return children; }
		Vector2Df GetMousePosition(const sf::Event& event) const;

		const Vector2Df& GetPosition() const { return position; }
		const Vector2Df& GetSize() const { return size; }
		bool GetIsHovered() const { return isHovered; }
		bool GetIsPressed() const { return isPressed; }
		Vector2Df GetAbsolutePosition() const;
		Vector2Df GetRelativePosition() const;

		void SetInteractive(bool interactive) { isInteractive = interactive; }
		bool IsInteractive() const { return isInteractive; }

		//=== Размеры и позиция ===//
		virtual Vector2Df CalculateDesiredSize() const;
		void SetPosition(const Vector2Df& newPosition);
		void SetRelativePosition(const Vector2Df& relativePos);
		virtual void SetSize(const Vector2Df& newSize);

		//=== Внешний вид ===//
		void SetVisibility(EWidgetVisibility newVisibility);
		EWidgetVisibility GetVisibility() const { return visibility; }
		bool IsVisible() const;

		//=== События ===//
		std::function<void()> OnHovered;
		std::function<void()> OnUnhovered;
		std::function<void()> OnPressed;
		std::function<void()> OnReleased;

	protected:
		//=== Вспомогательные методы ===//
		bool isUpdatingTransform = false;
		virtual bool IsPointInside(const Vector2Df& point) const;
		virtual void UpdateTransform();
		void UpdateChildrenTransforms();

		static void SetSpriteSize(sf::Sprite& sprite, float width, float height)
		{
			if (const sf::Texture* texture = sprite.getTexture())
			{
				sf::FloatRect bounds = sprite.getLocalBounds();
				if (bounds.width > 0 && bounds.height > 0)
				{
					// Сохраняем origin перед изменением scale
					sf::Vector2f oldOrigin = sprite.getOrigin();
					sprite.setScale(
						width / bounds.width,
						height / bounds.height);
					// Восстанавливаем origin
					sprite.setOrigin(oldOrigin);
				}
			}
		}

		//=== Графические компоненты ===//
		sf::Sprite sprite;
		bool useSprite = false;
		bool isInCanvas = false;
		friend class Canvas;

	private:
		Widget* parent = nullptr;
		std::vector<Widget*> children;

		EWidgetVisibility visibility = EWidgetVisibility::Visible;
		Vector2Df position;
		Vector2Df size;
		Vector2Df scale = { 1.0f, 1.0f };

		bool isHovered = false;
		bool isPressed = false;
		bool isInteractive = true;
	};
}; // namespace Engine
