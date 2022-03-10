#include "pch.h"

#include "Type.h"

namespace Next::Reflection
{
	bool
	Type::IsConvertibleTo(TypeId a_typeId) const
	{
		if (a_typeId == TypeId::Null)
		{
			return false;
		}

		if (m_typeId == a_typeId)
		{
			return true;
		}

		Type* parent = TryGet(m_baseTypeId);

		if (parent)
		{
			return parent->IsConvertibleTo(a_typeId);
		}

		return false;
	}

	bool
	Type::IsConvertibleTo(Type const* a_type) const
	{
		return IsConvertibleTo(a_type->GetTypeId());
	}

	bool
	Type::IsConvertibleTo(Type const& a_type) const
	{
		return IsConvertibleTo(a_type.GetTypeId());
	}

	bool
	Type::IsConvertibleFrom(TypeId a_typeId) const
	{
		if (a_typeId == TypeId::Null)
		{
			return false;
		}

		if (m_typeId == a_typeId)
		{
			return true;
		}

		// Check if the other type is convertible to this type
		Type* otherType = TryGet(a_typeId);
		
		if (otherType)
		{
			return otherType->IsConvertibleTo(m_typeId);
		}

		return false;
	}

	bool
	Type::IsConvertibleFrom(Type const* a_type) const
	{
		return IsConvertibleFrom(a_type->GetTypeId());
	}

	bool
	Type::IsConvertibleFrom(Type const& a_type) const
	{
		return IsConvertibleFrom(a_type.GetTypeId());
	}
}
