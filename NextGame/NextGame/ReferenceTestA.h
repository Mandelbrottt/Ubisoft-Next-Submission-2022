#pragma once

#include <HeaderInclude.h>

class ReferenceTestB;

class ReferenceTestA : public Next::Behaviour
{
	ReflectDeclare(ReferenceTestA, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	SetRefB(ReferenceTestB* a_refB);

private:
	ReferenceTestA* m_refA;
	ReferenceTestB* m_refB;

	ReflectMembers(
		ReflectField(m_refA)
		ReflectField(m_refB)
	)
};
