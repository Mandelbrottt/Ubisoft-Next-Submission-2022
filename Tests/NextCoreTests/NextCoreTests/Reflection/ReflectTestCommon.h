#pragma once

// Don't include NextCore/Reflection because it is already included in the precompiled header

namespace Reflection
{
	namespace TryGet
	{
		struct TestType final
		{
			ReflectDeclare(TestType)

		protected:
			TestType() = default;
			~TestType() = default;
		};
	}

	namespace Base
	{
		struct BaseType
		{
			ReflectDeclare(BaseType)

		protected:
			BaseType() = default;
			~BaseType() = default;
		};
		
		struct BadDerivedType : BaseType
		{
			// Don't declare the base type to illustrate that you need to
			ReflectDeclare(BadDerivedType)

		protected:
			BadDerivedType() = default;
			~BadDerivedType() = default;
		};
		
		struct GoodDerivedType : BaseType
		{
			ReflectDeclare(BaseType, BaseType)

		protected:
			GoodDerivedType() = default;
			~GoodDerivedType() = default;
		};

		struct NestedGoodDerivedType : GoodDerivedType
		{
			// Don't declare the base type to illustrate that you need to
			ReflectDeclare(NestedGoodDerivedType, GoodDerivedType)

		protected:
			NestedGoodDerivedType() = default;
			~NestedGoodDerivedType() = default;
		};
	}
}