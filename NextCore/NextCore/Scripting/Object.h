#pragma once

namespace NextCore::Scripting
{
	/**
	 * \brief The root base class for all objects represented in NextCore (eg. Entities, Components, etc.)
	 */
	class Object
	{
	protected:
		Object()  = default;

		virtual ~Object() = default;
	public:
	};
}
