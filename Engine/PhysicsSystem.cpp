#include "pch.h"
#include "PhysicsSystem.h"

namespace Engine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
        for (int i = 0; i < colliders.size(); i++)
        {
            auto bodyA = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
            if (!bodyA) continue;

            for (int j = 0; j < colliders.size(); j++)
            {
                if (j == i) continue;

                auto bodyB = colliders[j]->GetGameObject()->GetComponent<RigidbodyComponent>();
                if (!bodyB) continue;

                // ���������� �������� ����� ����� ��������������� ���������
                if (bodyA->GetKinematic() && bodyB->GetKinematic()) continue;

                sf::FloatRect intersection;
                if (colliders[i]->bounds.intersects(colliders[j]->bounds, intersection))
                {
                    if (colliders[i]->isTrigger != colliders[j]->isTrigger)
                    {
                        if (triggersEnteredPair.find(colliders[i]) == triggersEnteredPair.end() &&
                            triggersEnteredPair.find(colliders[j]) == triggersEnteredPair.end())
                        {
                            auto trigger = new Trigger(colliders[i], colliders[j]);
                            colliders[i]->OnTriggerEnter(*trigger);
                            colliders[j]->OnTriggerEnter(*trigger);

                            triggersEnteredPair.emplace(colliders[i], colliders[j]);
                        }
                    }
                    else if (!colliders[i]->isTrigger)
                    {
                        float intersectionWidth = intersection.width;
                        float intersectionHeight = intersection.height;
                        Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth,
                                                         intersection.top - 0.5f * intersectionHeight };

                        Vector2Df aPosition = { colliders[i]->bounds.left, colliders[i]->bounds.top };
                        auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
                        auto bTransform = colliders[j]->GetGameObject()->GetComponent<TransformComponent>();

                        // ����������, ����� ������ ��������������
                        bool aIsKinematic = bodyA->GetKinematic();
                        bool bIsKinematic = bodyB->GetKinematic();

                        // ���������, �������� �� ���� �� �������� �����
                        bool aIsFloor = colliders[i]->GetGameObject()->GetTag() == "Floor";
                        bool bIsFloor = colliders[j]->GetGameObject()->GetTag() == "Floor";

                        // ��������� ���������� ��������
                        if (!aIsKinematic && bIsKinematic)
                        {
                            // ������ A �� ��������������, ������ B ��������������
                            if (bIsFloor)
                            {
                                // ���� B - ���, �� ��������� �������� ������ �� ���������
                                if (intersectionPosition.y > aPosition.y)
                                {
                                    aTransform->MoveBy({ 0, -intersectionHeight });
                                }
                            }
                            else
                            {
                                // ��� ���� ���������� ������� ������
                                if (intersectionWidth > intersectionHeight)
                                {
                                    if (intersectionPosition.y > aPosition.y)
                                    {
                                        aTransform->MoveBy({ 0, -intersectionHeight });
                                    }
                                    else
                                    {
                                        aTransform->MoveBy({ 0, intersectionHeight });
                                    }
                                }
                                else
                                {
                                    if (intersectionPosition.x > aPosition.x)
                                    {
                                        aTransform->MoveBy({ -intersectionWidth, 0.f });
                                    }
                                    else
                                    {
                                        aTransform->MoveBy({ intersectionWidth, 0.f });
                                    }
                                }
                            }
                        }
                        else if (aIsKinematic && !bIsKinematic)
                        {
                            // ������ A ��������������, ������ B �� ��������������
                            if (aIsFloor)
                            {
                                // ���� A - ���, �� ��������� �������� ������ �� ���������
                                if (intersectionPosition.y > bTransform->GetWorldPosition().y)
                                {
                                    bTransform->MoveBy({ 0, -intersectionHeight });
                                }
                            }
                            else
                            {
                                // ��� ���� ���������� ������� ������
                                if (intersectionWidth > intersectionHeight)
                                {
                                    if (intersectionPosition.y > bTransform->GetWorldPosition().y)
                                    {
                                        bTransform->MoveBy({ 0, -intersectionHeight });
                                    }
                                    else
                                    {
                                        bTransform->MoveBy({ 0, intersectionHeight });
                                    }
                                }
                                else
                                {
                                    if (intersectionPosition.x > bTransform->GetWorldPosition().x)
                                    {
                                        bTransform->MoveBy({ -intersectionWidth, 0.f });
                                    }
                                    else
                                    {
                                        bTransform->MoveBy({ intersectionWidth, 0.f });
                                    }
                                }
                            }
                        }

                        auto collision = new Collision(colliders[i], colliders[j], intersection);
                        colliders[i]->OnCollision(*collision);
                        colliders[j]->OnCollision(*collision);
                    }
                }
            }
        }

		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				auto trigger = new Trigger(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(*trigger);
				triggeredPair->second->OnTriggerExit(*trigger);

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		std::cout << "Subscribe " << collider << std::endl;
		colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
	}
}