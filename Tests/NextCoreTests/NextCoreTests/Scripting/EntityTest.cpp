#include "pch.h"

#include "ScriptingTestCommon.h"

namespace Next
{
	extern Detail::Registry g_mainEntityRegistry;
}

namespace Scripting
{
	using namespace Next;

	TEST(Entity, EntityIdOperations)
	{
		entity_id_underlying_t lhs_underlying = 1;
		entity_id_underlying_t rhs_underlying = 2;

		EntityId lhs = static_cast<EntityId>(lhs_underlying);
		EntityId rhs = static_cast<EntityId>(rhs_underlying);

		EXPECT_FALSE(lhs == rhs);
		EXPECT_TRUE(lhs != rhs);

		rhs = lhs;

		EXPECT_TRUE(lhs == rhs);
		EXPECT_FALSE(lhs != rhs);
	}
	
	TEST(Entity, EntityCreation)
	{
		ScriptingTestStateInit();

		Entity entity1 = Entity::Create();
		EXPECT_TRUE(entity1.IsValid());

		Entity entity2 = Entity::Create();
		EXPECT_TRUE(entity2.IsValid());

		EXPECT_NE(entity1, entity2);

		Entity entity1Copy = entity1;
		EXPECT_TRUE(entity1.IsValid());
		
		EXPECT_EQ(entity1, entity1Copy);
	}

	TEST(Entity, EntityDestruction)
	{
		ScriptingTestStateInit();
		
		Entity entity1 = Entity::Create();
		Entity entity2 = Entity::Create();
		Entity entity1Copy = entity1;
		
		entity1.Destroy();

		// Destroying an entity doesn't modify the value of the entity value-type
		EXPECT_EQ(entity1, entity1Copy);

		// Both variables should be valid because destruction only queues for destruction at end of frame
		EXPECT_TRUE(entity1.IsValid());
		EXPECT_TRUE(entity1Copy.IsValid());
		EXPECT_TRUE(entity2.IsValid());

		Detail::SimulateEntityUpdate();

		// Both variables should be valid because destruction only queues for destruction at end of frame
		EXPECT_FALSE(entity1.IsValid());
		EXPECT_FALSE(entity1Copy.IsValid());
		EXPECT_TRUE(entity2.IsValid());

		entity1 = Entity::Create();
		entity1Copy = entity1;

		// Destroy the entity without queuing for deletion at end of frame
		entity1.DestroyImmediate();

		EXPECT_FALSE(entity1.IsValid());
		EXPECT_FALSE(entity1Copy.IsValid());
		EXPECT_TRUE(entity2.IsValid());
	}
}
