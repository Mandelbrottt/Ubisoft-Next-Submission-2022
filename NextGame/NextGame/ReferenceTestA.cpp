#include "ReferenceTestA.h"

#include "ReferenceTestB.h"

ReflectRegister(ReferenceTestA);

void
ReferenceTestA::OnCreate()
{
	m_refA = this;
}

void
ReferenceTestA::SetRefB(ReferenceTestB* a_refB)
{
	m_refB = a_refB;
}
