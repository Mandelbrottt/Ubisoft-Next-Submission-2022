#include "ReferenceTestB.h"

#include "ReferenceTestA.h"

ReflectRegister(ReferenceTestB);

void
ReferenceTestB::OnCreate()
{
	m_refB = this;
}

void
ReferenceTestB::SetRefA(ReferenceTestA* a_refA)
{
	m_refA = a_refA;
}
