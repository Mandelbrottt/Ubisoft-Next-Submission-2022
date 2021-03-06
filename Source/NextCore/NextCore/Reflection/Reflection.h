#pragma once

#include "Type.h"
#include "TypeTraits.h"

#include "NextCoreCommon.h"

#pragma region Macro Argument Overloading
    #define _CLEAR(x)
    #define _EXPAND(x) x
    #define _CAT(a, b) a##b
    #define _SELECT(name, num) _CAT(name##_, num)
    #define _GET_COUNT(_1, _2, _3, _4, _5, count, ...) count
    #define _VA_SIZE(...) _EXPAND(_GET_COUNT(__VA_ARGS__, 5, 4, 3, 2, 1))
    #define _VA_SELECT(name, ...) _EXPAND(_SELECT(name, _VA_SIZE(__VA_ARGS__))(__VA_ARGS__))
    #define _MACRO_OVERLOAD(name, ...) _EXPAND(_VA_SELECT(name, __VA_ARGS__))
#pragma endregion

// Reflection API inspired by https://www.educba.com/reflection-in-c-plus-plus/
#pragma region Reflection Macros
	#define _REFLECT_OFFSET_OF(_type_, _field_) ((size_t) &((_type_*) 0)->_field_)

	#define _REFLECT_NAMESPACE ::Next::Reflection::

	#define _REFLECT_CLASS_NAME _ReflectClassNameIdentifier

	#define _REFLECT_DECLARE_COMMON(_class_) \
		_MACRO_AUTO_FORMAT_INDENT \
	private: \
		friend class _REFLECT_NAMESPACE Type; \
		typedef _class_ _REFLECT_TYPE_ALIAS; \
		static constexpr char* _REFLECT_CLASS_NAME = #_class_; \
		\
		static \
		void \
		_ReflectType(_REFLECT_NAMESPACE Type& r) \
		{ \
			if constexpr (_REFLECT_NAMESPACE is_complete_type_v<_REFLECT_BASE_ALIAS>) \
			{ \
				r.operator()<_REFLECT_BASE_ALIAS, _REFLECT_TYPE_ALIAS>(); \
			} \
		} \
		\
	public: \
		struct _REFLECT_VALID_REFLECTION_TYPE_ALIAS {}; \
		static \
		_REFLECT_NAMESPACE Type& \
		GetStaticType() \
		{ \
			return _REFLECT_NAMESPACE Type::Get<_REFLECT_TYPE_ALIAS>();\
		} \
		virtual \
		_REFLECT_NAMESPACE Type& \
		GetType() \
		{ \
			return _REFLECT_NAMESPACE Type::Get<_REFLECT_TYPE_ALIAS>();\
		}

	#define _REFLECT_DECLARE_1(_class_) \
		_MACRO_AUTO_FORMAT_INDENT \
	private: \
		struct _REFLECT_BASE_ALIAS; \
		class  _REFLECT_BASE_ALIAS; \
		_REFLECT_DECLARE_COMMON(_class_)

	#define _REFLECT_DECLARE_2(_class_, _base_) \
		_REFLECT_DECLARE_COMMON(_class_) \
	private: \
		typedef _base_ _REFLECT_BASE_ALIAS; \
	public: \
		static \
		_REFLECT_NAMESPACE Type& \
		GetBaseType() \
		{ \
			return _REFLECT_NAMESPACE Type::Get<_REFLECT_BASE_ALIAS>();\
		}

	#define ReflectDeclare(...) _MACRO_OVERLOAD(_REFLECT_DECLARE, __VA_ARGS__)

	#define ReflectMembers(_list_) \
		_MACRO_AUTO_FORMAT_INDENT \
	private:\
		static \
		void \
		_ReflectMembers(_REFLECT_NAMESPACE Type& r) \
		{ \
			r _list_; \
		}

	#pragma region Reflect Field Overloads
		#define _REFLECT_FIELD_PROTOTYPE(_field_, ...) \
			(_REFLECT_NAMESPACE Field {\
				/*.debugName        =*/ #_field_,\
				/*.fieldName        =*/ _REFLECT_NAMESPACE Pick<_REFLECT_NAMESPACE Name>(__VA_ARGS__, _REFLECT_NAMESPACE Name(#_field_)).c_str,\
				/*.fieldDescription =*/ _REFLECT_NAMESPACE Pick<_REFLECT_NAMESPACE Description>(__VA_ARGS__, _REFLECT_NAMESPACE Description("")).c_str,\
				/*.offset           =*/ (uint32_t) _REFLECT_OFFSET_OF(_REFLECT_TYPE_ALIAS, _field_), \
				/*.size             =*/ (uint32_t) sizeof(_field_), \
				/*.fieldType        =*/ _REFLECT_NAMESPACE GetTypeId<decltype(_field_)>(), \
				/*.containingType   =*/ _REFLECT_NAMESPACE GetTypeId<_REFLECT_TYPE_ALIAS>(),\
				/*.flags            =*/ _REFLECT_NAMESPACE Pick<_REFLECT_NAMESPACE FieldFlags>(__VA_ARGS__, _REFLECT_NAMESPACE FieldFlags::None)\
			})

		#define _REFLECT_FIELD_1(_field_) _REFLECT_FIELD_2(_field_, #_field_)

		#define _REFLECT_FIELD_2(_field_, _arg1_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_)

		#define _REFLECT_FIELD_3(_field_, _arg1_, _arg2_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_)

		#define _REFLECT_FIELD_4(_field_, _arg1_, _arg2_, _arg3_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_, _arg3_)

		#define _REFLECT_FIELD_5(_field_, _arg1_, _arg2_, _arg3_, _arg4_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_, _arg3_, _arg4_)

		#define _REFLECT_FIELD_6(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)

		#define _REFLECT_FIELD_7(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)

		#define _REFLECT_FIELD_8(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
			_REFLECT_FIELD_PROTOTYPE(_field_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)

		#define ReflectField(...) _MACRO_OVERLOAD(_REFLECT_FIELD, __VA_ARGS__)

	#pragma endregion
	#define ReflectRegister(_type_) \
		extern "C" volatile _REFLECT_NAMESPACE TypeId _REFLECT_REGISTER_##_type_ = []() \
		{ \
			_REFLECT_NAMESPACE Type::Register<_type_>();\
			return _REFLECT_NAMESPACE GetTypeId<_type_>(); \
		}()
#pragma endregion