#pragma once

#include "Graphics/Color.h"

#include "Scripting/Component.h"

namespace Next
{
	enum class LightType : uint8_t
	{
		Directional,
		Spot,
	};
	
	class Light : public Component
	{
		ReflectDeclare(Light, Component)
			
	public:
		Color ambientColor = Color(0, 0, 0);
		Color diffuseColor = Color(1, 1, 1);

		CONSUMER_DEPRECATED("Specular calculations currently unsupported")
		Color specularColor = Color(1, 1, 1);

		LightType type = LightType::Spot;

		ReflectMembers(
			ReflectField(ambientColor)
			ReflectField(diffuseColor)
			ReflectField(specularColor)
			ReflectField(type)
		)
	};
}