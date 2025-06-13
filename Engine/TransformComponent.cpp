#include "pch.h"
#include "TransformComponent.h"
#include <cmath>
#include <iostream>

namespace Engine
{
	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject) {}

	void TransformComponent::Update(float deltaTime)
	{

	}
	void TransformComponent::Render()
	{

	}

	void TransformComponent::MoveBy(const Vector2Df& offset)
	{
		MoveBy(offset.x, offset.y);
		isUpdated = false;
	}
	void TransformComponent::MoveBy(float offsetX, float offsetY)
	{
		localPosition.x += offsetX;
		localPosition.y += offsetY;
		isUpdated = false;
	}
	void TransformComponent::SetWorldPosition(const Vector2Df& position)
	{
		SetWorldPosition(position.x, position.y);
		isUpdated = false;
	}
	void TransformComponent::SetWorldPosition(float positionX, float positionY)
	{
		Vector2Df newPosition = { positionX, positionY };
		if (parent == nullptr)
		{
			localPosition = newPosition;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(newPosition, GetWorldRotate(), GetWorldScale());
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(localTransform);

			isUpdated = false;
			updateLocalTransform();
		}
	}
	void TransformComponent::SetLocalPosition(const Vector2Df& position)
	{
		SetLocalPosition(position.x, position.y);
		isUpdated = false;
	}
	void TransformComponent::SetLocalPosition(float positionX, float positionY)
	{
		localPosition.x = positionX;
		localPosition.y = positionY;
		isUpdated = false;
	}

	void TransformComponent::RotateBy(float offset)
	{
		localRotation = localRotation + offset;
		while (localRotation >= 360)
		{
			localRotation -= 360;
		}
		while (localRotation < 0)
		{
			localRotation += 360;
		}
		isUpdated = false;
	}
	void TransformComponent::SetWorldRotation(float angle)
	{
		if (parent == nullptr)
		{
			localRotation = angle;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), angle, GetWorldScale());
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(localTransform);

			isUpdated = false;
			updateLocalTransform();
		}
	}
	void TransformComponent::SetLocalRotation(float angle)
	{
		localRotation = angle;
		while (localRotation >= 360)
		{
			localRotation -= 360;
		}
		while (localRotation < 0)
		{
			localRotation += 360;
		}
		isUpdated = false;
	}

	void TransformComponent::ScaleBy(const Vector2Df& scale)
	{
		ScaleBy(scale.x, scale.y);
		isUpdated = false;
	}
	void TransformComponent::ScaleBy(float scaleX, float scaleY)
	{
		localScale.x *= scaleX;
		localScale.y *= scaleY;
		isUpdated = false;
	}
	void TransformComponent::SetWorldScale(const Vector2Df& scale)
	{
		SetWorldScale(scale.x, scale.y);
		isUpdated = false;
	}
	void TransformComponent::SetWorldScale(float scaleX, float scaleY)
	{
		Vector2Df newScale = { scaleX, scaleY };
		if (parent == nullptr)
		{
			localScale = newScale;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), GetWorldRotate(), newScale);
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(localTransform);

			isUpdated = false;
			updateLocalTransform();
		}
	}
	void TransformComponent::SetLocalScale(const Vector2Df& scale)
	{
		SetLocalScale(scale.x, scale.y);
		isUpdated = false;
	}
	void TransformComponent::SetLocalScale(float scaleX, float scaleY)
	{
		localScale.x = scaleX;
		localScale.y = scaleY;
		isUpdated = false;
	}

	const Vector2Df& TransformComponent::GetWorldPosition() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			return GetLocalPosition();
		}

		setWorldInfoFrom(parent->GetWorldTransform() * localTransform);
		return position;
	}
	const Vector2Df& TransformComponent::GetLocalPosition() const
	{
		updateLocalTransform();
		return localPosition;
	}

	const float TransformComponent::GetWorldRotate() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			return GetLocalRotation();
		}

		setWorldInfoFrom(parent->GetWorldTransform() * localTransform);
		return rotation;
	}

	const float TransformComponent::GetLocalRotation() const
	{
		updateLocalTransform();
		return localRotation;
	}

	const Vector2Df& TransformComponent::GetWorldScale() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			return GetLocalScale();
		}

		setWorldInfoFrom(parent->GetWorldTransform() * localTransform);
		return scale;
	}
	const Vector2Df& TransformComponent::GetLocalScale() const
	{
		updateLocalTransform();
		return localScale;
	}

	void TransformComponent::SetParent(TransformComponent* newParent)
	{
		if (parent == nullptr && newParent != nullptr)
		{
			newParent->gameObject->AddChild(gameObject);
			localTransform = newParent->GetWorldTransform().GetInversed() * localTransform;
		}
		else if (parent != nullptr && newParent == nullptr)
		{
			parent->gameObject->RemoveChild(gameObject);
			localTransform = parent->GetWorldTransform() * localTransform;
		}
		else if (parent != nullptr && newParent != nullptr)
		{
			parent->gameObject->RemoveChild(gameObject);
			newParent->gameObject->AddChild(gameObject);
			localTransform = newParent->GetWorldTransform().GetInversed() * (parent->GetWorldTransform() * localTransform);
		}

		setLocalInfoFrom(localTransform);

		parent = newParent;
		isUpdated = false;
	}
	TransformComponent* TransformComponent::GetParent() const
	{
		return parent;
	}

	const Matrix2D TransformComponent::GetWorldTransform() const
	{
		updateLocalTransform();

		if (parent == nullptr)
		{
			return localTransform;
		}

		return parent->GetWorldTransform() * localTransform;
	}

	void TransformComponent::Print() const
	{
		std::cout << "Transform name : " << gameObject->GetName() << std::endl;

		std::cout << "Position" << std::endl;
		std::cout << "  World: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ")" << std::endl;
		std::cout << "  Local: (" << GetLocalPosition().x << ", " << GetLocalPosition().y << ")" << std::endl;

		std::cout << "Rotation" << std::endl;
		std::cout << "  World: " << GetWorldRotate() << std::endl;
		std::cout << "  Local: " << GetLocalRotation() << std::endl;

		std::cout << "Scale" << std::endl;
		std::cout << "  World: (" << GetWorldScale().x << ", " << GetWorldScale().y << ")" << std::endl;
		std::cout << "  Local: (" << GetLocalScale().x << ", " << GetLocalScale().y << ")" << std::endl;
	}

	void TransformComponent::setWorldInfoFrom(const Matrix2D& transform) const
	{
		position.x = transform.GetMatrix()[0][2];
		position.y = transform.GetMatrix()[1][2];

		scale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		scale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);

		rotation = std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265;
	}
	void TransformComponent::setLocalInfoFrom(const Matrix2D& transform) const
	{
		localPosition.x = transform.GetMatrix()[0][2];
		localPosition.y = transform.GetMatrix()[1][2];

		localScale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		localScale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);

		localRotation = std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265;
	}
	void TransformComponent::updateLocalTransform() const
	{
		if (!isUpdated)
		{
			localTransform = createTransform(localPosition, localRotation, localScale);
			isUpdated = true;
		}
	}
	void TransformComponent::updateLocalTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const
	{
		if (!isUpdated)
		{
			localTransform = createTransform(position, rotation, scale);
			isUpdated = true;
		}
	}
	Matrix2D TransformComponent::createTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const
	{
		return Matrix2D(position, rotation, scale);
	}
}