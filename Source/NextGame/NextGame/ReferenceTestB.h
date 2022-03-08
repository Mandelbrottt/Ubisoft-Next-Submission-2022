#pragma once

#include <HeaderInclude.h>

class ReferenceTestA;

class ReferenceTestB : public Next::Behaviour
{
	ReflectDeclare(ReferenceTestB, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	SetRefA(ReferenceTestA* a_refA);

private:
	ReferenceTestA* m_refA;
	ReferenceTestB* m_refB;
	
	ReflectMembers(
		ReflectField(m_refA)
		ReflectField(m_refB)
	)
};
