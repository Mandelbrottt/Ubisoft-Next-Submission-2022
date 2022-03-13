#include "pch.h"
#include "Registry.h"

#include "Scripting/Component.h"

namespace Next::Detail
{
	void
	Registry::Reset()
	{
		m_nextEntityId = EntityId::FirstValid;

		m_activeEntities = {};
		m_entityInfos = {};
		
		m_componentPoolInfos = {};
	}

	EntityId
	Registry::OnCreateEntity()
	{
		EntityId newEntityId = IncrementEntityCounter();

		m_activeEntities.insert(newEntityId);
		m_entityInfos[newEntityId] = {};
		
		return newEntityId;
	}

	void
	Registry::OnDestroyEntity(EntityId a_entityId)
	{
		m_activeEntities.erase(a_entityId);
		m_entityInfos.erase(a_entityId);

		for (auto& [typeId, poolInfo] : m_componentPoolInfos)
		{
			poolInfo.pool.RemoveComponent(a_entityId);
		}
	}

	void
	Registry::OnFirstUpdate(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		auto iter = m_activeEntities.find(a_entityId);

		if (iter == m_activeEntities.end())
		{
			return;
		}

		ComponentPool* pool = EnsureComponentPoolInstantiated(a_typeId);

		if (!pool)
		{
			return;
		}

		auto* component = pool->GetComponent(a_entityId);
		if (component)
		{
			component->OnFirstUpdate();
		}
	}
	
	void
	Registry::OnUpdate()
	{
		for (auto entityId : m_activeEntities)
		{
			if (!IsActive(entityId))
			{
				continue;
			}
			
			for (auto& [typeId, poolInfo] : m_componentPoolInfos)
			{
				auto* component = poolInfo.pool.GetComponent(entityId);

				if (!component || !component->IsActive())
				{
					continue;
				}
				
				component->OnUpdate();
			}
		}
	}

	// Circumvent warning for returning temporary by reference, find a way to not do this
	static std::string g_getNameReturn;

	std::string const&
	Registry::GetName(EntityId a_entityId)
	{
		auto iter = m_entityInfos.find(a_entityId);

		if (iter == m_entityInfos.end())
		{
			return g_getNameReturn;
		}

		return iter->second.name;
	}

	void
	Registry::SetName(EntityId a_entityId, std::string_view const& a_name)
	{
		auto iter = m_entityInfos.find(a_entityId);

		if (iter == m_entityInfos.end())
		{
			return;
		}

		iter->second.name = std::string(a_name);
	}

	bool
	Registry::IsActive(EntityId a_entityId) const
	{
		auto iter = m_entityInfos.find(a_entityId);

		if (iter == m_entityInfos.end())
		{
			return false;
		}

		return iter->second.active;
	}

	void
	Registry::SetActive(EntityId a_entityId, bool a_active)
	{
		auto iter = m_entityInfos.find(a_entityId);

		if (iter == m_entityInfos.end())
		{
			return;
		}

		iter->second.active = a_active;
	}

	bool
	Registry::IsValid(EntityId a_entityId) const
	{
		// Is find() or count() faster on average?
		return m_activeEntities.find(a_entityId) != m_activeEntities.end();
		//return m_activeEntities.count(a_entityId) != 0;
	}

	Component*
	Registry::AddComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return AddComponent(a_entityId, a_type.GetTypeId());
	}

	Component*
	Registry::AddComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);

		if (!componentPool)
		{
			return nullptr;
		}
		
		auto result = componentPool->AddComponent(a_entityId);
		
		return result;
	}

	bool
	Registry::RemoveComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return RemoveComponent(a_entityId, a_type.GetTypeId());
	}

	bool
	Registry::RemoveComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);
		
		if (!componentPool)
		{
			return nullptr;
		}
		
		InvalidateReferencesToComponent(a_entityId, a_typeId);

		auto result = componentPool->RemoveComponent(a_entityId);
		
		return result;
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return GetComponent(a_entityId, a_type.GetTypeId());
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);
		
		if (!componentPool)
		{
			return nullptr;
		}
		
		auto result = componentPool->GetComponent(a_entityId);
		
		return result;
	}

	Component const*
	Registry::GetComponent(EntityId a_entityId, Reflection::Type const& a_type) const
	{
		return const_cast<Registry*>(this)->GetComponent(a_entityId, a_type);
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId) const
	{
		return const_cast<Registry*>(this)->GetComponent(a_entityId, a_typeId);
	}

	std::vector<Component*>
	Registry::GetAllComponentsForEntity(EntityId a_entityId) const
	{
		std::vector<Component*> result;
		GetAllComponentsForEntity(a_entityId, result);
		return result;
	}

	void
	Registry::GetAllComponentsForEntity(EntityId a_entityId, std::vector<Component*>& a_outVector) const
	{
		a_outVector.clear();

		for (auto const& poolInfos : m_componentPoolInfos)
		{
			auto& componentPool = poolInfos.second.pool;

			// Const cast because we're not modifying the pool itself so the function can still be const
			Component* component = const_cast<Component*>(componentPool.GetComponent(a_entityId));
			if (component)
			{
				a_outVector.push_back(component);
			}
		}
	}

	EntityId
	Registry::IncrementEntityCounter()
	{
		auto result = m_nextEntityId;

		auto rawEntityId = static_cast<entity_id_underlying_t>(m_nextEntityId);

		rawEntityId++;
		m_nextEntityId = static_cast<EntityId>(rawEntityId);

		return result;
	}

	ComponentPool*
	Registry::EnsureComponentPoolInstantiated(Reflection::TypeId a_typeId)
	{
		auto iter = m_componentPoolInfos.find(a_typeId);

		if (iter == m_componentPoolInfos.end())
		{
			auto* type = Reflection::Type::TryGet(a_typeId);
	
			if (!type)
			{
				fprintf(
					stderr,
					"[Next::Registry::EnsureComponentPoolInstantiated] "
					"Type with id [%u] could not be found!",
					static_cast<Reflection::type_id_underlying_t>(a_typeId)
				);
				return nullptr;
			}

			auto componentPool = ComponentPool(type->GetFactory());
			componentPool.SetResizeCallbacks(this, &Registry::CacheAllReferencesOfType, &Registry::RestoreAllReferencesOfType);

			ComponentPoolInfo poolInfo = {
				std::move(componentPool),
				{}
			};
			
			auto pair = std::make_pair(a_typeId, std::move(poolInfo));
			iter = m_componentPoolInfos.emplace(std::move(pair)).first;

			GenerateComponentReferenceTrackingInfo(a_typeId);
		}
		
		return &iter->second.pool;
	}

	void
	Registry::GenerateComponentReferenceTrackingInfo(Reflection::TypeId a_typeId)
	{
		using Reflection::Type;
		using Reflection::TypeId;
		
		Type const* referenceType = Type::TryGet(a_typeId);
		Type const& componentType = Type::Get<Component>();
		
		assert(referenceType);
		
		for (auto const& field : referenceType->GetInstanceFields())
		{
			if (!componentType.IsConvertibleFrom(field.fieldTypeId))
			{
				continue;
			}
			
			EnsureComponentPoolInstantiated(field.fieldTypeId);

			auto& poolInfo = m_componentPoolInfos.at(field.fieldTypeId);

			auto& fieldsToRefWatch = poolInfo.fieldsToRefWatch;

			fieldsToRefWatch.push_back(&field);
		}
	}

	static std::vector<Component*> g_componentsList;

	/* HACK: The following three functions should be refactored, but because their innermost scope has very
	         different requirements from each-other, it is very difficult to refactor them in a way that isn't
	         super verbose. For now just copy and paste and deal with it */
	void
	Registry::InvalidateReferencesToComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		// Retrieve the component pool info that contains all the types that depend on referenceType
		auto& resizedPoolInfo = m_componentPoolInfos.at(a_typeId);

		auto& fields = resizedPoolInfo.fieldsToRefWatch;
		
		// Iterate through all of the fields that are of pointer type to referenceType
		for (auto const* field : fields)
		{
			// Get the type containing the field and its corresponding componentPoolInfo
			auto containingTypeId = field->containingTypeId;
			auto& containingPoolInfo = m_componentPoolInfos.at(containingTypeId);

			containingPoolInfo.pool.GetAllComponents(&g_componentsList);
			
			// Iterate through all of the components and store the entityId of their references
			for (auto* containingComponent : g_componentsList)
			{
				// GetValue returns a pointer to a pointer, dereference it to get the actual component pointer
				Component const* referenceComponent = 
					*static_cast<Component* const*>(field->GetValue(containingComponent));

				if (referenceComponent == nullptr)
				{
					continue;
				}

				// Find all references to the component being destroyed and null them out
				auto referenceEntityId = referenceComponent->GetEntityId();

				if (referenceEntityId == a_entityId)
				{
					// Set the reference pointer to nullptr since the component doesn't exist anymore
					field->SetValue(containingComponent, nullptr);
				}
			}
		}
	}

	struct ReferenceInfo
	{
		Reflection::TypeId containingTypeId;
		EntityId containingEntityId;

		void const* referencePtrValue;
		EntityId referenceEntityId;
	};

	std::vector<ReferenceInfo> g_referenceInfos;
	
	void
	Registry::CacheAllReferencesOfType(Reflection::TypeId a_referenceTypeId)
	{
		// Retrieve the component pool info that contains all the types that depend on referenceType
		auto& resizedPoolInfo = m_componentPoolInfos.at(a_referenceTypeId);

		auto& fields = resizedPoolInfo.fieldsToRefWatch;

		// Iterate through all of the fields that are of pointer type to referenceType
		for (auto const* field : fields)
		{
			// Get the type containing the field and its corresponding componentPoolInfo
			auto containingTypeId = field->containingTypeId;
			auto& containingPoolInfo = m_componentPoolInfos.at(containingTypeId);

			containingPoolInfo.pool.GetAllComponents(&g_componentsList);

			// Iterate through all of the components and store the entityId of their references
			for (auto* containingComponent : g_componentsList)
			{
				auto containingEntityId = containingComponent->GetEntityId();

				// GetValue returns a pointer to a pointer, dereference it to get the actual component pointer
				Component const* referenceComponent = 
					*static_cast<Component* const*>(field->GetValue(containingComponent));

				if (referenceComponent == nullptr)
				{
					continue;
				}

				// Retrieve the entityId of the reference
				auto referenceEntityId = referenceComponent->GetEntityId();

				// We need to store the current pointer value so that we can
				// value check against it in the restore function
				ReferenceInfo something = {
					containingTypeId,
					containingEntityId,
					referenceComponent,
					referenceEntityId
				};

				g_referenceInfos.push_back(something);
			}
		}

		// Sort the referenceInfos so that we can more efficiently iterate over the list
		auto sortPredicate = [](ReferenceInfo const& a_lhs, ReferenceInfo const& a_rhs)
		{
			auto lhs = static_cast<entity_id_underlying_t>(a_lhs.containingTypeId);
			auto rhs = static_cast<entity_id_underlying_t>(a_rhs.containingTypeId);

			return lhs < rhs;
		};

		std::sort(g_referenceInfos.begin(), g_referenceInfos.end(), sortPredicate);
	}

	void
	Registry::RestoreAllReferencesOfType(Reflection::TypeId a_referenceTypeId)
	{
		// Retrieve the component pool info that contains all the types that depend on referenceType
		auto& resizedPoolInfo = m_componentPoolInfos.at(a_referenceTypeId);

		auto& fields = resizedPoolInfo.fieldsToRefWatch;
		
		// Iterate through all of the fields that are of pointer type to referenceType
		for (auto const* field : fields)
		{
			// Get the type containing the field and its corresponding componentPoolInfo
			auto containingTypeId = field->containingTypeId;
			auto& containingPoolInfo = m_componentPoolInfos.at(containingTypeId);

			containingPoolInfo.pool.GetAllComponents(&g_componentsList);
			
			// Re-iterate through all of the components and reconstruct their references
			for (auto* containingComponent : g_componentsList)
			{
				auto containingEntityId = containingComponent->GetEntityId();
				
				// GetValue returns a pointer to a pointer, dereference it to get the actual component pointer
				void const* referencePtrValue = 
					*static_cast<Component* const*>(field->GetValue(containingComponent));

				// Search the reference infos list for the correct reference info
				auto findPredicate = [&](ReferenceInfo const& a_value)
				{
					return a_value.containingTypeId == containingTypeId &&
					       a_value.containingEntityId == containingEntityId &&
					       a_value.referencePtrValue == referencePtrValue;
				};

				auto iter = std::find_if(g_referenceInfos.begin(), g_referenceInfos.end(), findPredicate);

				if (iter == g_referenceInfos.end())
				{
					continue;
				}

				// Restore the reference by invalidating the component pointer and setting the field in the component
				Component* referenceComponent = GetComponent(iter->referenceEntityId, a_referenceTypeId);
				field->SetValue(containingComponent, &referenceComponent, sizeof(referenceComponent));
			}
		}
	}
}
