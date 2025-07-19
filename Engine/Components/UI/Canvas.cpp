#include "pch.h"
#include "Canvas.h"
#include "Layout.h"

namespace Engine
{
	Canvas::Canvas(GameObject* gameObject)
		: Widget(gameObject)
	{
		renderTarget = &RenderSystem::Instance()->GetMainWindow();
	}

	void Canvas::Update(float deltaTime)
	{
		for (auto& layer : layeredWidgets)
		{
			if (layer.widget && layer.widget->IsVisible())
			{
				layer.widget->Update(deltaTime);
			}
		}
	}

	void Canvas::Render()
	{
		if (!IsVisible() || !renderTarget)
			return;

		auto oldView = renderTarget->getView();

		if (isScreenSpace)
		{
			sf::View screenView(sf::FloatRect(0, 0, renderTarget->getSize().x, renderTarget->getSize().y));
			renderTarget->setView(screenView);
		}

		// Render layered widgets
		for (auto& layer : layeredWidgets)
		{
			if (layer.widget && layer.widget->IsVisible())
			{
				layer.widget->Render(); // Call regular Render() instead of RenderWidgetOnly()
			}
		}

		if (isScreenSpace)
		{
			renderTarget->setView(oldView);
		}
	}

	void Canvas::HandleEvent(const sf::Event& event)
	{
		if (!IsVisible())
			return;

		for (auto it = layeredWidgets.rbegin(); it != layeredWidgets.rend(); ++it)
		{
			if (!it->widget || !it->widget->IsValid())
			{
				it = decltype(it)(layeredWidgets.erase(std::next(it).base()));
				continue;
			}

			if (it->widget && it->widget->IsVisible())
			{
				it->widget->HandleEvent(event);

				if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
				{
					break;
				}
			}
		}
	}

	void Canvas::SetRenderTarget(sf::RenderTarget* target)
	{
		renderTarget = target;
	}

	void Canvas::SetScreenSpace(bool screenSpace)
	{
		isScreenSpace = screenSpace;
	}

	bool Canvas::IsScreenSpace() const
	{
		if (isScreenSpace)
			return true;
		return false;
	}

	void Canvas::AddToLayer(Widget* widget, int layer)
	{
		if (!widget || widget == this)
			return;

		RemoveFromLayer(widget);

		widget->isInCanvas = true;
		layeredWidgets.push_back({ layer, widget });

		UpdateWidhetTransform(widget);

		SortLayers();
	}

	void Canvas::RemoveFromLayer(Widget* widget)
	{
		layeredWidgets.erase(
			std::remove_if(layeredWidgets.begin(), layeredWidgets.end(),
				[widget](const WidgetLayer& l) { return l.widget == widget; }),
			layeredWidgets.end());
	}

	void Canvas::UpdateTransform()
	{
		if (renderTarget)
		{
			auto size = renderTarget->getSize();
			SetSize(Vector2Df(static_cast<float>(size.x), static_cast<float>(size.y)));
		}

		// Обновляем трансформации всех виджетов
		for (auto& layer : layeredWidgets)
		{
			UpdateWidhetTransform(layer.widget);
		}
	}

	void Canvas::SortLayers()
	{
		std::sort(layeredWidgets.begin(), layeredWidgets.end(),
			[](const WidgetLayer& a, const WidgetLayer& b) {
				return a.layer < b.layer;
			});
	}

	void Canvas::UpdateWidhetTransform(Widget* widget)
	{
		if (widget)
		{
			// Only set absolute position if widget isn't in a layout
			if (!widget->GetParent() || dynamic_cast<Layout*>(widget->GetParent()) == nullptr)
			{
				widget->SetPosition(GetPosition() + widget->GetPosition());
			}
			// For widgets in layouts, the layout handles their positioning
		}
	}

} // namespace Engine
