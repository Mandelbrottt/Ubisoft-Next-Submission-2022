#include "AccelerationBasedMover.h"

#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/Character/Player/PlayerShipController.h"
#include "Scripts/Objects/CollidableSphereTag.h"

ReflectRegister(AccelerationBasedMover);

using namespace Next;

void
AccelerationBasedMover::SetClamp(Vector3 a_clamp)
{
	m_clamp = a_clamp;
}

void
AccelerationBasedMover::ApplyAcceleration(Vector3 a_acceleration)
{
	m_velocity += a_acceleration * Time::DeltaTime();
}

void
AccelerationBasedMover::Move()
{
	m_velocity.x = std::clamp(m_velocity.x, -m_clamp.x, m_clamp.x);
	m_velocity.y = std::clamp(m_velocity.y, -m_clamp.y, m_clamp.y);
	m_velocity.z = std::clamp(m_velocity.z, -m_clamp.z, m_clamp.z);
	
	auto position = Transform()->GetPosition();

	position += m_velocity * Time::DeltaTime();

	if (PlayerShipController::flatPlanet)
	{
		auto min_y = PlayerShipController::min_y;
                                                       
		if (position.y < min_y)                               
		{                                                     
		    position.y = min_y;
			auto health = GetComponent<Health>();
			if (health && Math::Square(m_velocity.y) > Math::Square(10))
			{
				health->SubtractHealth();
			}
			
		    m_velocity.y = -m_velocity.y * 0.2f;              
		}                                                     
	}

	Transform()->SetPosition(position);
}

void
AccelerationBasedMover::OnTriggerCollisionStart(Collider* a_other)
{
	auto tag = a_other->GetComponent<CollidableSphereTag>();

	if (!tag)
	{
		return;
	}
	
	auto position = Transform()->GetPosition();
	auto planetPosition = a_other->Transform()->GetPosition();

	Vector3 planetToThis = position - planetPosition;

	auto health = GetComponent<Health>();
	if (health && m_velocity.MagnitudeSquared() > Math::Square(10))
	{
		health->SubtractHealth();
	}
	
	planetToThis.Normalize();
	
	auto otherSphereCollider = static_cast<SphereCollider*>(a_other);
	
	position = planetToThis * (otherSphereCollider->radius);
	
	m_velocity = -m_velocity * 0.5f;
	
	Transform()->SetPosition(position);
}

void
AccelerationBasedMover::OnTriggerCollision(Collider* a_other)
{
	auto tag = a_other->GetComponent<CollidableSphereTag>();

	if (!tag)
	{
		return;
	}
	
	auto position = Transform()->GetPosition();
	auto planetPosition = a_other->Transform()->GetPosition();

	Vector3 planetToThis = position - planetPosition;
	Vector3 nextPlanetToThis = (position + m_velocity * Time::DeltaTime()) - planetToThis;

	if (Vector::MagnitudeSquared(planetToThis) < Vector::MagnitudeSquared(nextPlanetToThis))
	{
		return;
	}
	
	planetToThis.Normalize();
	
	auto otherSphereCollider = static_cast<SphereCollider*>(a_other);
	
	position = planetToThis * (otherSphereCollider->radius + 0.05f);

	m_velocity = Vector3::Zero();
	
	Transform()->SetPosition(position);
}
