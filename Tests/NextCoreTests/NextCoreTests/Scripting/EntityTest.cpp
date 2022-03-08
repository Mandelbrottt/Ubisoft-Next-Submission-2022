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

		Entity entity;

		EXPECT_FALSE(entity.IsValid());
	}

	TEST(Entity, EntityDestruction)
	{
		ScriptingTestStateInit();
		
		Entity entity1 = Entity::Create();
		Entity entity2 = Entity::Create();
		Entity entity1Copy = entity1;
		
		entity1.Destroy();

		// Destroying an entity doesn't modify the value of the entity opaque type
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

	TEST(Entity, Naming)
	{
		ScriptingTestStateInit();

		const char* name1 = "Name1";
		const char* name2 = "Name2";

		Entity entity1 = Entity::Create(name1);
		Entity entity2 = Entity::Create(name2);

		EXPECT_EQ(entity1.GetName(), name1);
		EXPECT_EQ(entity2.GetName(), name2);

		entity1.SetName(name2);

		EXPECT_NE(entity1.GetName(), name1);
		EXPECT_EQ(entity2.GetName(), name2);

		entity1.DestroyImmediate();

		EXPECT_NE(entity1.GetName(), name1);
		EXPECT_EQ(entity1.GetName(), std::string());
		EXPECT_EQ(entity2.GetName(), name2);

		Entity entity1Copy = entity1;

		EXPECT_EQ(entity1.GetName(), entity1Copy.GetName());
	}
	
	TEST(Entity, FindByName)
	{
		ScriptingTestStateInit();

		const char* name1 = "Name1";
		const char* name2 = "Name2";

		// Create entity with one name
		Entity::Create(name1);

		Entity found = Entity::FindByName(name1);

		EXPECT_TRUE(found.IsValid());

		// Find an entity that doesn't exist
		found = Entity::FindByName(name2);

		EXPECT_FALSE(found.IsValid());

		// Create a duplicate named entity
		Entity::Create(name1);

		std::vector<Entity> foundEntities;
		Entity::FindAllByName(name1, foundEntities);

		EXPECT_EQ(foundEntities.size(), 2);

		for (auto const& entity : foundEntities)
		{
			EXPECT_TRUE(entity.IsValid());

			EXPECT_EQ(name1, entity.GetName());
		}

		// FindAllByName for a name where no entities exist with that name
		Entity::FindAllByName(name2, foundEntities);

		EXPECT_EQ(foundEntities.size(), 0);
	}
}
