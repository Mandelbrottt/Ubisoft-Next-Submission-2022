#pragma once

#include <HeaderInclude.h>

namespace Scripting
{
	using namespace Next;

	class RefTestB;

	class RefTestA : public Behaviour
	{
		ReflectDeclare(RefTestA, Behaviour)

	public:
		RefTestA* refA;
		RefTestB* refB;

		ReflectMembers(
			ReflectField(refA)
			ReflectField(refB)
		)
	};

	class RefTestB : public Behaviour
	{
		ReflectDeclare(RefTestB, Behaviour)

	public:
		RefTestB* refB;
		RefTestA* refA;

		ReflectMembers(
			ReflectField(refB)
			ReflectField(refA)
		)
	};
}