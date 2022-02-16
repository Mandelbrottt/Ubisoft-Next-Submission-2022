#include "CrazyLineThing.h"

#include <Application/Time.h>

#include <Components/LineRenderer.h>

using namespace NextCore;
using namespace Component;

/*
 *
 * //------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float        r = 1.0f;
	float        g = 1.0f;
	float        b = 1.0f;
	a += 10 * Time::DeltaTime();
	for (int i = 0; i < 20; i++)
	{
		float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
		float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
		float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
		float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
		g        = (float) i / 20.0f;
		b        = (float) i / 20.0f;
		App::DrawLine(sx, sy, ex, ey, r, g, b);
	}
 */

void
CrazyLineThing::OnCreate()
{
	auto* lineRenderer = AddComponent<LineRenderer>();
	lineRenderer->SetPointCount(40);
	lineRenderer->SetLineType(LineType::Separate);
}

void
CrazyLineThing::OnUpdate()
{
	static float a = 0.0f;
	float        r = 1.0f;
	float        g = 1.0f;
	float        b = 1.0f;
	a += 10 * Time::DeltaTime();
	
	auto* lineRenderer = GetComponent<LineRenderer>();
	for (int i = 0; i < lineRenderer->GetPointCount() / 2; i++)
	{
		float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
		float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
		float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
		float ey = 700 - cosf(a + i * 0.1f) * 60.0f;

		g = static_cast<float>(i) / 20.0f;
		b = static_cast<float>(i) / 20.0f;

		LineRenderer::Point p1 = {
			{ sx, sy },
			{ r, g, b }
		};
		LineRenderer::Point p2 = {
			{ ex, ey },
			{ r, g, b }
		};

		int adjustedIndex = i * 2;

		lineRenderer->SetPoint(adjustedIndex,     p1);
		lineRenderer->SetPoint(adjustedIndex + 1, p2);
	}
}
