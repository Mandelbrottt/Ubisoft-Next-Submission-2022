#pragma once

namespace NextCore::Scripting
{
	/**
	 * \brief The root base class for all objects represented in NextCore (eg. Entities, Components, etc.)
	 */
	class Object
	{
	protected:
		Object()          = default;
		virtual ~Object() = default;

		// Object's should only me referenced or moved, not copied
		Object(Object const& a_other) = delete;
		
		Object&
		operator =(Object const& a_other) = delete;
	public:
		Object(Object&& a_other) = default;
	};
}
