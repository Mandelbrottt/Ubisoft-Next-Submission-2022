#include "pch.h"

#include "ReferenceTrackingTestClasses.h"
#include "ScriptingTestCommon.h"
	
namespace Scripting
{
	using namespace Next;

	TEST(Component, AddComponent)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		auto ref1_a = entity1.AddComponent<RefTestA>();
		auto ref1_b = entity1.AddComponent<RefTestB>();
		
		Entity entity2 = Entity::Create();
		auto ref2_a = entity2.AddComponent<RefTestA>();
		auto ref2_b = entity2.AddComponent<RefTestB>();

		EXPECT_EQ(ref1_a, entity1.GetComponent<RefTestA>());
		EXPECT_EQ(ref1_b, entity1.GetComponent<RefTestB>());
		
		EXPECT_EQ(ref2_a, entity2.GetComponent<RefTestA>());
		EXPECT_EQ(ref2_b, entity2.GetComponent<RefTestB>());

		EXPECT_NE(ref1_a, ref2_a);
		EXPECT_NE(ref1_b, ref2_b);
	}

	TEST(Component, AddComponentReflection)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		auto ref1_a = entity1.AddComponent(RefTestA::GetStaticType());
		auto ref1_b = entity1.AddComponent(Reflection::Type::Get<RefTestB>());
		
		Entity entity2 = Entity::Create();
		auto ref2_a = entity2.AddComponent(ref1_a->GetType());
		auto ref2_b = entity2.AddComponent(Reflection::GetTypeId<RefTestB>());

		EXPECT_EQ(ref1_a, entity1.GetComponent<RefTestA>());
		EXPECT_EQ(ref1_b, entity1.GetComponent<RefTestB>());
		
		EXPECT_EQ(ref2_a, entity2.GetComponent<RefTestA>());
		EXPECT_EQ(ref2_b, entity2.GetComponent<RefTestB>());

		EXPECT_NE(ref1_a, ref2_a);
		EXPECT_NE(ref1_b, ref2_b);
	}
	
	TEST(Component, RemoveComponent)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		auto ref1_a = entity1.AddComponent<RefTestA>();
		
		Entity entity2 = Entity::Create();
		auto ref2_b = entity2.AddComponent<RefTestB>();

		EXPECT_EQ(ref1_a,  entity1.GetComponent<RefTestA>());
		EXPECT_EQ(nullptr, entity1.GetComponent<RefTestB>());
		
		EXPECT_EQ(nullptr, entity2.GetComponent<RefTestA>());
		EXPECT_EQ(ref2_b, entity2.GetComponent<RefTestB>());
		entity1.RemoveComponent<RefTestA>();
		entity2.RemoveComponent<RefTestB>();
		
		EXPECT_NE(ref1_a,  entity1.GetComponent<RefTestA>());
		EXPECT_NE(ref2_b, entity2.GetComponent<RefTestB>());

		EXPECT_EQ(nullptr, entity1.GetComponent<RefTestA>());
		EXPECT_EQ(nullptr, entity2.GetComponent<RefTestB>());
	}

	TEST(Component, RemoveComponentReflection)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		auto ref1_a = entity1.AddComponent(RefTestA::GetStaticType());
		
		Entity entity2 = Entity::Create();
		auto ref2_b = entity2.AddComponent(Reflection::Type::Get<RefTestB>());

		EXPECT_EQ(ref1_a,  entity1.GetComponent<RefTestA>());
		EXPECT_EQ(nullptr, entity1.GetComponent<RefTestB>());
		
		EXPECT_EQ(nullptr, entity2.GetComponent<RefTestA>());
		EXPECT_EQ(ref2_b, entity2.GetComponent<RefTestB>());
		entity1.RemoveComponent(Reflection::GetTypeId<RefTestA>());
		entity2.RemoveComponent(ref2_b->GetType());
		
		EXPECT_NE(ref1_a, entity1.GetComponent<RefTestA>());
		EXPECT_NE(ref2_b, entity2.GetComponent<RefTestB>());

		EXPECT_EQ(nullptr, entity1.GetComponent<RefTestA>());
		EXPECT_EQ(nullptr, entity2.GetComponent<RefTestB>());
	}
	
	TEST(Component, DestroyEntityRemovesComponents)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		auto ref1_a = entity1.AddComponent<RefTestA>();
		auto ref1_b = entity1.AddComponent<RefTestB>();
		
		Entity entity2 = Entity::Create();
		auto ref2_a = entity2.AddComponent<RefTestA>();
		auto ref2_b = entity2.AddComponent<RefTestB>();

		entity1.DestroyImmediate();

		EXPECT_NE(entity1.GetComponent<RefTestA>(), ref1_a);
		EXPECT_NE(entity1.GetComponent<RefTestB>(), ref1_b);
		
		EXPECT_EQ(entity2.GetComponent<RefTestA>(), ref2_a);
		EXPECT_EQ(entity2.GetComponent<RefTestB>(), ref2_b);
		
		EXPECT_EQ(entity1.GetComponent<RefTestA>(), nullptr);
		EXPECT_EQ(entity1.GetComponent<RefTestB>(), nullptr);

		EXPECT_NE(entity2.GetComponent<RefTestA>(), nullptr);
		EXPECT_NE(entity2.GetComponent<RefTestB>(), nullptr);
	}
	
	TEST(Component, ReferenceTracking)
	{
		ScriptingTestStateInit();

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
		for (int i = 0; i < Detail::g_componentPoolDefaultSize; i++)
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
		EXPECT_NE(oldRefA_A, refA->refA);
		EXPECT_NE(oldRefA_B, refA->refB);
		EXPECT_NE(oldRefB_A, refA->refA);
		EXPECT_NE(oldRefB_B, refA->refB);

		// Check that all references to the removed RefTestA are set to nullptr
		// NOTE: refB doesn't have to be invalidated here because RefTestB ComponentPool is untouched
		entityA.RemoveComponent<RefTestA>();
		EXPECT_EQ(refB->refA, nullptr);

		// Cleanup after ourselves
		for (auto& entity : entities)
		{
			entity.Destroy();
		}

		entityA.Destroy();
		entityB.Destroy();
	}
}
