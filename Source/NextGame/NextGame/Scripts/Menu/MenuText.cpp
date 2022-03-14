#include "MenuText.h"

ReflectRegister(MenuText);

void
MenuText::OnCreate()
{
	m_titleText = "Planetar";
	m_startText = "Press A to Start";
}

void
MenuText::OnUpdate()
{
	Next::Gui::Print(m_titleText, { 0, 0 });

	Next::Gui::Print(m_startText, { 0, -0.25 });
}
