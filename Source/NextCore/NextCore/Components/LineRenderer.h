#pragma once

#include "Graphics/Color.h"

#include "Math/Vector3.h"

#include "Scripting/Component.h"

void Render();

namespace Next
{
	enum class LineType
	{
		Continuous,
		Separate,
		Triangle,
	};
}

namespace Next
{
	// Not currently supported
	class LineRenderer : public Component
	{
		friend void ::Render();
		
		ComponentDeclare(LineRenderer, Component)

	public:
		struct Point
		{
			Vector3 point;
			Color   color = Color::white;
		};

	public:
		[[nodiscard]]
		std::size_t
		GetPointCount() const;

		// Make return type std::optional?
		[[nodiscard]]
		Point
		GetPoint(int a_index, bool* a_success = nullptr) const;
		
		[[nodiscard]]
		LineType
		GetLineType() const;

		void
		SetPointCount(int a_count);

		void
		SetPoint(int a_index, Point a_point, bool* a_success = nullptr);
		
		void
		SetLineType(LineType a_lineType);

		bool
		IsIndexInBounds(int a_index) const;

	private:
		void
		OnRender() const;

		void
		OnRenderContinuous() const;
	
		void
		OnRenderSeparate() const;
	
		void
		OnRenderTriangle() const;
	
	private:
		std::vector<Point> m_points;

		LineType m_lineType = LineType::Continuous;
		
		ReflectMembers(
			ReflectField(m_points)
		)
	};
}
