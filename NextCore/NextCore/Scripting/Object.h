#pragma once

#include <string>

#include "Reflection/Reflection.h"

namespace Next
{
	/**
	 * \brief The root base class for all objects represented in NextCore (eg. Entities, Components, etc.)
	 */
	class Object
	{
	protected:
		struct ObjectConstructionArgs
		{
			const char* typeName;
		};

		explicit
		Object(ObjectConstructionArgs const& a_args)
		#ifndef NEXT_RELEASE
			: m_typeName(a_args.typeName)
		#endif
		{ }

		virtual ~Object() = default;

		// Objects should only me referenced or moved, not copied
		Object(Object const& a_other) = delete;

		Object&
		operator =(Object const& a_other) = delete;
	public:
		Object(Object&& a_other) = default;
	
	#ifndef NEXT_RELEASE
	protected:
		const char*
		GetName() const
		{
			return m_typeName;
		}
	
	private:
		const char* m_typeName;
	#endif

		REFLECT_DECLARE(Object)
	};
}
