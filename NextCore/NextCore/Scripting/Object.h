#pragma once

#include <string>

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
		#ifdef NEXT_DEBUG
			:
			m_typeName(a_args.typeName)
		#endif
		{ }

		virtual ~Object() = default;

		// Objects should only me referenced or moved, not copied
		Object(Object const& a_other) = delete;

		Object&
		operator =(Object const& a_other) = delete;
	public:
		Object(Object&& a_other) = default;
	
	#ifdef NEXT_DEBUG
	protected:
		const char*
		GetName() const
		{
			return m_typeName;
		}
	
	private:
		const char* m_typeName;
	#endif
	};
}
