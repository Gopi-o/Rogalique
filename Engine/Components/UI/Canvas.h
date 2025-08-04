#pragma once
#include <Components/UI/Widget.h>
#include <Systems/Render/RenderSystem.h>

namespace Engine
{
	class Canvas : public Widget
	{
	public:
		Canvas(GameObject* gameObject);

		// Основные методы
		void Update(float deltaTime) override;
		void Render() override;
		void HandleEvent(const sf::Event& event) override;

		void SetRenderTarget(sf::RenderTarget* target);
		sf::RenderTarget* GetRenderTarget() const { return renderTarget; }

		void SetScreenSpace(bool screenSpace);
		bool IsScreenSpace() const;

		void AddToLayer(Widget* widget, int layer = 0);
		void RemoveFromLayer(Widget* widget);

	protected:
		void UpdateTransform() override;

	private:
		sf::RenderTarget* renderTarget = nullptr;
		bool isScreenSpace = true;
		struct WidgetLayer
		{
			int layer;
			Widget* widget;
		};

		std::vector<WidgetLayer> layeredWidgets;

		void SortLayers();
		void UpdateWidhetTransform(Widget* widget);
	};
} // namespace Engine
