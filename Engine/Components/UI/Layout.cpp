#include "pch.h"
#include "Layout.h"

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

} // namespace Engine