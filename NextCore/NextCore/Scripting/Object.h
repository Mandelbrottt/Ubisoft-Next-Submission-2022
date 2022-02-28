#pragma once

#include <string>

#include "Reflection/Reflection.h"

#define GenerateObjectConstructors(_class_) \
		_MACRO_AUTO_FORMAT_INDENT \
	protected: \
		explicit \
		_class_(const char* a_name) \
			: Base({ a_name }) { } \
		~_class_() override = default; \
	public: \
		_class_() \
			: _class_( #_class_ ) {} \
	private: \
		_MACRO_AUTO_FORMAT_INDENT

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

#ifndef NEXT_CORE
	#undef GenerateObjectConstructors
#endif