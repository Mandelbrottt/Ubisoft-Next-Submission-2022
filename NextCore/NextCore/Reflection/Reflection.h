#pragma once

#include "Type.h"

#pragma region Macro Argument Overloading
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

	#define _REFLECT_TYPE_ALIAS This
	#define _REFLECT_BASE_ALIAS Base

	#define _REFLECT_DECLARE_COMMON(_class_) \
		private: \
			friend class ::NextCore::Reflection::Type; \
			typedef _class_ _REFLECT_TYPE_ALIAS; \
		public: \
			static ::NextCore::Reflection::Type& GetType() \
			{ \
				return ::NextCore::Reflection::Type::Get<_REFLECT_TYPE_ALIAS>();\
			} \
			static ::NextCore::Reflection::Type* TryGetType() \
			{ \
				return ::NextCore::Reflection::Type::TryGet<_REFLECT_TYPE_ALIAS>();\
			}

	#define _REFLECT_DECLARE_1(_class_) \
		struct Base; \
		class  Base; \
		_REFLECT_DECLARE_COMMON(_class_)

	#define _REFLECT_DECLARE_2(_derived_, _base_) \
		_REFLECT_DECLARE_COMMON(_derived_) \
		private: \
			typedef _base_ _REFLECT_BASE_ALIAS; \
		public: \
			static ::NextCore::Reflection::Type& GetBaseType() \
			{ \
				return ::NextCore::Reflection::Type::Get<_REFLECT_BASE_ALIAS>();\
			} \
			static ::NextCore::Reflection::Type* TryGetBaseType() \
			{ \
				return ::NextCore::Reflection::Type::TryGet<_REFLECT_BASE_ALIAS>();\
			}

	#define REFLECT_DECLARE(...) _MACRO_OVERLOAD(_REFLECT_DECLARE, __VA_ARGS__)

	#define REFLECT_MEMBERS(_list_) \
		private:\
			friend class ::NextCore::Reflection::Type; \
			static \
			void \
			Reflect(::NextCore::Reflection::Type& r) \
			{ \
				r _list_; \
				\
				if constexpr (::NextCore::Reflection::is_complete_type_v<Base>)\
				{\
					r.operator()<Base, This>();\
				}\
			}

	#pragma region Reflect Field Overloads
		#define _REFLECT_FIELD_PROTOTYPE(_field_, ...) \
			(::NextCore::Reflection::Field {\
				/*.debugName        =*/ #_field_,\
				/*.fieldName        =*/ ::NextCore::Reflection::Pick<::NextCore::Reflection::Name>(__VA_ARGS__, ::NextCore::Reflection::Name(#_field_)).c_str,\
				/*.fieldDescription =*/ ::NextCore::Reflection::Pick<::NextCore::Reflection::Description>(__VA_ARGS__, ::NextCore::Reflection::Description("")).c_str,\
				/*.offset           =*/ (uint32_t) _REFLECT_OFFSET_OF(_REFLECT_TYPE_ALIAS, _field_), \
				/*.size             =*/ (uint32_t) sizeof(_field_), \
				/*.fieldType        =*/ typeid(decltype(_field_)), \
				/*.containingType   =*/ typeid(_REFLECT_TYPE_ALIAS),\
				/*.flags            =*/ ::NextCore::Reflection::Pick<::NextCore::Reflection::FieldFlags>(__VA_ARGS__, ::NextCore::Reflection::FfNone)\
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

		#define REFLECT_FIELD(...) _MACRO_OVERLOAD(_REFLECT_FIELD, __VA_ARGS__)

	#pragma endregion
	#define REFLECT_REGISTER(_type_) \
		static int _REFLECT_REGISTER_##_type_ = []() \
		{ \
			::NextCore::Reflection::Type::Register<_type_>();\
			return ::NextCore::Reflection::GetStaticId<_type_>(); \
		}();
#pragma endregion
