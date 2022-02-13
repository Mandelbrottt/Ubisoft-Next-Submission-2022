#include "pch.h"

#include "Type.h"

namespace NextCore::Reflection
{
	uint32_t Type::s_staticIdCounter;
	
	std::unordered_map<int, Type> Type::s_types;
}
