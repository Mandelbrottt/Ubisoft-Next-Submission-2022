#pragma once

#include "Graphics/Color.h"

#include "Math/Vector3.h"

#include "Scripting/Component.h"

// TEMPORARY:
void GameRender();

namespace NextCore::Graphics
{
	enum class LineType
	{
		Continuous,
		Separate,
		Triangle,
	};
}

namespace NextCore::Component
{
	using Graphics::Color;
	using Graphics::LineType;
	using Math::Vector3;

	class LineRenderer : public Scripting::Component
	{
		friend void ::GameRender();

		GenerateConstructors(LineRenderer)

	public:
		struct Point
		{
			Vector3 point;
			Color   color = Color::white;
		};

	public:
		std::size_t
		GetPointCount() const;

		// Make return type std::optional?
		Point
		GetPoint(int a_index, bool* a_success = nullptr) const;
		
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

	public:
		REFLECT_DECLARE(LineRenderer, Component)

		REFLECT_MEMBERS(
			REFLECT_FIELD(m_points)
		)
	};
}
