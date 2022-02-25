#include "pch.h"

#include "LineRenderer.h"

#include <NextAPI/app.h>

namespace Next
{
	REFLECT_REGISTER(LineRenderer);

	using Point = LineRenderer::Point;

	std::size_t
	LineRenderer::GetPointCount() const
	{
		return m_points.size();
	}

	static
	void
	SetIfNotNull(bool* a_ptr, bool a_value)
	{
		if (a_ptr)
		{
			*a_ptr = a_value;
		}
	}

	Point
	LineRenderer::GetPoint(int a_index, bool* a_success) const
	{
		bool isValid = IsIndexInBounds(a_index);

		SetIfNotNull(a_success, isValid);
		
		if (!isValid)
		{
			return {};
		}
		
		return m_points.at(a_index);
	}

	LineType
	LineRenderer::GetLineType() const
	{
		return m_lineType;
	}

	void
	LineRenderer::SetLineType(LineType a_lineType)
	{
		m_lineType = a_lineType;
	}

	void
	LineRenderer::SetPointCount(int a_count)
	{
		if (a_count == m_points.size())
		{
			return;
		}

		m_points.resize(a_count);
	}

	void
	LineRenderer::SetPoint(int a_index, Point a_point, bool* a_success)
	{
		bool isValid = IsIndexInBounds(a_index);

		SetIfNotNull(a_success, isValid);
		
		if (!isValid)
		{
			return;
		}

		m_points[a_index] = a_point;
	}

	void
	LineRenderer::OnRender() const
	{
		if (m_points.size() < 2)
		{
			return;
		}

		switch (m_lineType)
		{
			case LineType::Continuous:
				OnRenderContinuous();
				break;
			case LineType::Separate:
				OnRenderSeparate();
				break;
			case LineType::Triangle:
				OnRenderTriangle();
				break;
		}
	}

	void
	LineRenderer::OnRenderContinuous() const
	{
		for (int i = 0; i < m_points.size() - 1; i++)
		{
			const auto& [p1, c1] = m_points.at(i);
			const auto& [p2, c2]  = m_points.at(i + 1);

			auto lerp = [](float a_t, float a_p1, float a_p2)
			{
				return a_p1 * (1 - a_t) + a_p2 * a_t;
			};
			
			float r = lerp(0.5, c1.r, c2.r);
			float g = lerp(0.5, c1.g, c2.g);
			float b = lerp(0.5, c1.b, c2.b);
			
			App::DrawLine(p1.x, p1.y, p2.x, p2.y, r, g, b);
		}
	}

	void
	LineRenderer::OnRenderSeparate() const
	{
		for (int i = 0; i < m_points.size() / 2; i++)
		{
			int adjustedIndex = 2 * i;
			
			const auto& [p1, c1] = m_points.at(adjustedIndex);
			const auto& [p2, c2] = m_points.at(adjustedIndex + 1);

			auto lerp = [](float a_t, float a_p1, float a_p2)
			{
				return a_p1 * (1 - a_t) + a_p2 * a_t;
			};
			
			float r = lerp(0.5, c1.r, c2.r);
			float g = lerp(0.5, c1.g, c2.g);
			float b = lerp(0.5, c1.b, c2.b);
			
			App::DrawLine(p1.x, p1.y, p2.x, p2.y, r, g, b);
		}
	}

	void
	LineRenderer::OnRenderTriangle() const
	{
		throw "Not Implemented";
	}

	bool
	LineRenderer::IsIndexInBounds(int a_index) const
	{
		return a_index >= 0 && a_index < m_points.size();
	}
}
