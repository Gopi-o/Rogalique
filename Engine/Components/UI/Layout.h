#pragma once
#include <Components/UI/Widget.h>
#include <Math/Vector.h>

namespace Engine
{
	class Layout : public Widget
	{
	public:
		Layout(GameObject* gameObject);

		void SetSpacing(float newSpacing);
		float GetSpacing() const { return spacing; }

		void SetPadding(const Vector2Df& newPadding);
		const Vector2Df& GetPadding() const { return padding; }

		void SetAutoSize(bool enable);
		bool GetAutoSize() const { return autoSize; }

		void AddChild(Widget* child) override;
		void RemoveChild(Widget* child) override;
		void SetSize(const Vector2Df& newSize) override;

	protected:
		virtual void UpdateLayout() = 0;
		void UpdateTransform() override;

		float spacing = 5.0f;
		bool autoSize = true;
		Vector2Df padding = { 10.0f, 10.0f };

	private:
		bool needsLayoutUpdate = false;
	};

	class VerticalBox : public Layout
	{
	public:
		VerticalBox(GameObject* gameObject);

		void UpdateLayout() override;

		void AddChild(Widget* child) override;
		void RemoveChild(Widget* child) override;

	private:
		void CalculateLayout();
	};

	class HorizontalBox : public Layout
	{
	public:
		HorizontalBox(GameObject* gameObject);

		void UpdateLayout() override;

		void AddChild(Widget* child) override;
		void RemoveChild(Widget* child) override;

	private:
		void CalculateLayout();
	};

	class Grid : public Layout
	{
	public:
		Grid(GameObject* gameObject);

		void SetColumns(int count);
		void SetRows(int count);

		void UpdateLayout() override;

		void AddChild(Widget* child) override;
		void RemoveChild(Widget* child) override;

	private:
		void CalculateLayout();

		int columns = 1;
		int rows = 1;
	};
} // namespace Engine
