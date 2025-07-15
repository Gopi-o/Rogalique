#include "pch.h"
#include "Layout.h"
#include "Button.h"
#include <Systems/Logger.h>

namespace Engine
{
	Layout::Layout(GameObject* gameObject)
		: Widget(gameObject)
	{
	}

	void Layout::SetSpacing(float newSpacing)
	{
		if (spacing != newSpacing)
		{
			spacing = newSpacing;
			needsLayoutUpdate = true;
		}
	}
	void Layout::SetPadding(const Vector2Df& newPadding)
	{
		if (padding != newPadding)
		{
			padding = newPadding;
			needsLayoutUpdate = true;
		}
	}
	void Layout::SetAutoSize(bool enable)
	{
		if (autoSize != enable)
		{
			autoSize = enable;
			needsLayoutUpdate = true;
		}
	}

	void Layout::AddChild(Widget* child)
	{
		Widget::AddChild(child);
		needsLayoutUpdate = true;
	}

	void Layout::RemoveChild(Widget* child)
	{
		Widget::RemoveChild(child);
		needsLayoutUpdate = true;
	}

	void Layout::SetSize(const Vector2Df& newSize)
	{
		if (!autoSize)
		{
			Widget::SetSize(newSize);
			needsLayoutUpdate = true;
		}
	}

	void Layout::UpdateTransform()
	{
		Widget::UpdateTransform();

		if (needsLayoutUpdate)
		{
			UpdateLayout();
			needsLayoutUpdate = false;
		}
	}

	VerticalBox::VerticalBox(GameObject* gameObject)
		: Layout(gameObject)
	{
	}

	void VerticalBox::UpdateLayout()
	{
		CalculateLayout();
	}

	void VerticalBox::AddChild(Widget* child)
	{
		Layout::AddChild(child);
		CalculateLayout();
	}

	void VerticalBox::RemoveChild(Widget* child)
	{
		Layout::RemoveChild(child);
		CalculateLayout();
	}

	HorizontalBox::HorizontalBox(GameObject* gameObject)
		: Layout(gameObject)
	{
	}

	void HorizontalBox::UpdateLayout()
	{
		CalculateLayout();
	}

	void HorizontalBox::AddChild(Widget* child)
	{
		Layout::AddChild(child);
		CalculateLayout();
	}

	void HorizontalBox::RemoveChild(Widget* child)
	{
		Layout::RemoveChild(child);
		CalculateLayout();
	}

	void HorizontalBox::CalculateLayout()
	{
		float currentX = padding.x;
		const float availableHeight = GetSize().y - padding.y * 2;

		for (auto* child : GetChildren())
		{
			if (!child || child->GetVisibility() == EWidgetVisibility::Collapsed)
				continue;

			const auto desiredSize = child->CalculateDesiredSize();
			const float childWidth = desiredSize.x;
			const float childHeight = std::min(desiredSize.y, availableHeight);

			child->SetRelativePosition({ currentX, padding.y });
			child->SetSize({ childWidth, childHeight });

			currentX += childWidth + spacing;
		}

		if (autoSize)
		{
			Widget::SetSize({ currentX - spacing + padding.x, GetSize().y });
		}
	}

	Grid::Grid(GameObject* gameObject)
		: Layout(gameObject)
	{
	}

	void Grid::SetColumns(int count)
	{
		columns = std::max(1, count);
		CalculateLayout();
	}

	void Grid::SetRows(int count)
	{
		rows = std::max(1, count);
		CalculateLayout();
	}

	void Grid::UpdateLayout()
	{
		CalculateLayout();
	}

	void Grid::AddChild(Widget* child)
	{
		Layout::AddChild(child);
		CalculateLayout();
	}

	void Grid::RemoveChild(Widget* child)
	{
		Layout::RemoveChild(child);
		CalculateLayout();
	}

	void Grid::CalculateLayout()
	{
		float currentY = padding.y;
		const float availableWidth = GetSize().x - padding.x * 2;

		for (auto* child : GetChildren())
		{
			if (!child || child->GetVisibility() == EWidgetVisibility::Collapsed)
				continue;
			const auto desiredSize = child->CalculateDesiredSize();
			const float childWidth = std::min(desiredSize.x, availableWidth);
			const float childHeight = desiredSize.y;

			child->SetPosition({ padding.x, currentY });
			child->SetSize({ childWidth, childHeight });

			currentY += childHeight + spacing;
		}
	}

	void VerticalBox::CalculateLayout()
	{
		if (isUpdatingTransform)
			return;
		isUpdatingTransform = true;

		float currentY = padding.y;
		const float availableWidth = GetSize().x - padding.x * 2;

		for (auto* child : GetChildren())
		{
			if (!child || child->GetVisibility() == EWidgetVisibility::Collapsed)
				continue;

			// ѕолучаем желаемый размер или используем стандартный
			Vector2Df desiredSize = child->CalculateDesiredSize();
			float childWidth = desiredSize.x > 0 ? desiredSize.x : availableWidth;
			float childHeight = desiredSize.y > 0 ? desiredSize.y : 60.f; // —тандартна€ высота

			// ќграничиваем максимальной доступной шириной
			childWidth = std::min(childWidth, availableWidth);

			child->SetRelativePosition({ padding.x, currentY });

			// ”станавливаем новый размер только если он отличаетс€
			if (child->GetSize().x != childWidth || child->GetSize().y != childHeight)
			{
				child->SetSize({ childWidth, childHeight });
			}

			currentY += childHeight + spacing;
		}

		if (autoSize)
		{
			const float newHeight = currentY - spacing + padding.y;
			if (GetSize().y != newHeight)
			{
				Widget::SetSize({ GetSize().x, newHeight });
			}
		}

		isUpdatingTransform = false;
	}

} // namespace Engine