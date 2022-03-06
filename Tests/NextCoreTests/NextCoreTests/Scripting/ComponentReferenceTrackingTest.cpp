#include "pch.h"

extern int g_componentPoolDefaultSize;

namespace Next
{
	extern Detail::Registry g_mainEntityRegistry;
}
	
namespace Scripting
{
	using namespace Next;

	class RefTestB;

	class RefTestA : public Behaviour
	{
		ReflectDeclare(RefTestA, Behaviour)

	public:
		RefTestA* refA;
		RefTestB* refB;
		
		ReflectMembers(
			ReflectField(refA)
			ReflectField(refB)
		)
	};

	class RefTestB : public Behaviour
	{
		ReflectDeclare(RefTestB, Behaviour)

	public:
		RefTestB* refB;
		RefTestA* refA;

		ReflectMembers(
			ReflectField(refB)
			ReflectField(refA)
		)
	};

	ReflectRegister(RefTestA);
	ReflectRegister(RefTestB);

	TEST(Component, ReferenceTracking)
	{
		// Reset the registry
		g_mainEntityRegistry.Reset();

		// Create the initial entities
		Entity entityA = Entity::Create();
		auto* refA = entityA.AddComponent<RefTestA>();

		Entity entityB = Entity::Create();
		auto* refB = entityB.AddComponent<RefTestB>();

		// Assign references to each other
		refA->refA = refA;
		refA->refB = refB;

		refB->refB = refB;
		refB->refA = refA;

		// Store the current pointers for later checking
		RefTestA* oldRefA_A = refA->refA;
		RefTestB* oldRefA_B = refA->refB;
		
		RefTestA* oldRefB_A = refB->refA;
		RefTestB* oldRefB_B = refB->refB;
		
		std::vector<Entity> entities;

		// Create enough entities that the ComponentPool is forced to resize
		for (int i = 0; i < g_componentPoolDefaultSize; i++)
		{
			Entity entity = Entity::Create();

			entity.AddComponent<RefTestA>();
			entity.AddComponent<RefTestB>();

			entities.push_back(std::move(entity));
		}

		// Existing local pointers are invalidated on resize.
		// Never maintain local pointers after a call to AddComponent.
		refA = entityA.GetComponent<RefTestA>();
		refB = entityB.GetComponent<RefTestB>();

		// Check that the pointer members of the components changed
		ASSERT_NE(oldRefA_A, refA->refA);
		ASSERT_NE(oldRefA_B, refA->refB);
		ASSERT_NE(oldRefB_A, refA->refA);
		ASSERT_NE(oldRefB_B, refA->refB);

		// Check that all references to the removed RefTestA are set to nullptr
		// NOTE: refB doesn't have to be invalidated here because RefTestB ComponentPool is untouched
		entityA.RemoveComponent<RefTestA>();
		ASSERT_EQ(refB->refA, nullptr);

		// Cleanup after ourselves
		for (auto& entity : entities)
		{
			entity.Destroy();
		}

		entityA.Destroy();
		entityB.Destroy();
	}
}
