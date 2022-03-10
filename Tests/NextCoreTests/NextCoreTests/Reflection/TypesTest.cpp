#include "pch.h"

#include "ReflectTestCommon.h"

namespace Reflection
{
	using namespace Next;
	
	namespace Reflection = Next::Reflection;
	
	TEST(Type, TryGetType)
	{
		using Reflection::Type;
		
		auto        typeId          = Reflection::GetTypeId<TryGet::TestType>();
		const char* unqualifiedName = "TestType";
		const char* qualifiedName   = "Reflection::TryGet::TestType";

		auto typeFromTypeId            = Type::TryGet(typeId);
		auto typeFromUnqualifiedString = Type::TryGet(unqualifiedName);
		auto typeFromQualifiedString   = Type::TryGet(qualifiedName);

		// The type is not registered, shouldn't find the type
		EXPECT_EQ(typeFromTypeId,            nullptr);
		EXPECT_EQ(typeFromUnqualifiedString, nullptr);
		EXPECT_EQ(typeFromQualifiedString,   nullptr);

		// Register the type with the type system
		Type::Register<TryGet::TestType>();
			
		typeFromTypeId            = Type::TryGet(typeId);
		typeFromUnqualifiedString = Type::TryGet(unqualifiedName);
		typeFromQualifiedString   = Type::TryGet(qualifiedName);

		// Type is now registered, should find the type
		EXPECT_NE(typeFromTypeId,            nullptr);
		EXPECT_NE(typeFromQualifiedString,   nullptr);

		// Unqualified string should still return nullptr
		// You need to supply the fully-qualified name of the type
		EXPECT_EQ(typeFromUnqualifiedString, nullptr);

		EXPECT_EQ(typeFromTypeId->GetTypeId(),          typeId);
		EXPECT_EQ(typeFromQualifiedString->GetTypeId(), typeId);
	}

	TEST(Type, GetBaseType)
	{
		using Reflection::Type;

		// Automatically registers these types since templates are at compile time	
		auto& baseType               = Type::Get<Base::BaseType>();
		auto& badDerivedType         = Type::Get<Base::BadDerivedType>();
		auto& goodDerivedType        = Type::Get<Base::GoodDerivedType>();
		auto& nestedGoodDerivedType  = Type::Get<Base::NestedGoodDerivedType>();

		EXPECT_NE(baseType.GetTypeId(), badDerivedType.GetBaseTypeId());
		EXPECT_EQ(baseType.GetTypeId(), goodDerivedType.GetBaseTypeId());

		// NestedGooDerivedType isn't an immediate child of baseType
		EXPECT_NE(baseType.GetTypeId(), nestedGoodDerivedType.GetBaseTypeId());

		// IsConvertibleTo/From checks the whole inheritance chain
		EXPECT_FALSE(baseType.IsConvertibleFrom(badDerivedType));
		EXPECT_TRUE(baseType.IsConvertibleFrom(goodDerivedType));
		EXPECT_TRUE(baseType.IsConvertibleFrom(nestedGoodDerivedType));

		EXPECT_FALSE(badDerivedType.IsConvertibleTo(baseType));
		EXPECT_TRUE(goodDerivedType.IsConvertibleTo(baseType));
		EXPECT_TRUE(nestedGoodDerivedType.IsConvertibleTo(baseType));
	}
}
