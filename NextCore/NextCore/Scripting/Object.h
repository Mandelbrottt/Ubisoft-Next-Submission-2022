#pragma once

#include <string>

#include "Reflection/Reflection.h"
//
//#define GenerateObjectConstructors(_class_) \
//		_MACRO_AUTO_FORMAT_INDENT \
//	_class_() \
//		: Base({ #_class_ }) {} \
//	protected: \
//		explicit \
//		_class_(const char* a_name) \
//			: Base({ a_name }) { } \
//		~_class_() override = default; \

namespace Next
{
	/**
	 * \brief The root base class for all objects represented in NextCore (eg. Entities, Components, etc.)
	 */
	class Object
	{
		ReflectDeclareNoConstructors(Object)

	protected:
		struct ObjectConstructionArgs { };

		Object() = default;
		
		explicit
		Object(ObjectConstructionArgs const& a_args) { }

		virtual ~Object() = default;

		// Objects should only me referenced or moved, not copied
		Object(Object const& a_other) = delete;

		Object&
		operator =(Object const& a_other) = delete;
	public:
		Object(Object&& a_other) = default;
	};
}

//#ifndef NEXT_CORE
//	#undef GenerateObjectConstructors
//#endif