#pragma once

namespace NextCore::Reflection
{
	struct GenericConstructor
	{
		bool valid = false;

		virtual
		~GenericConstructor()
		{
			valid = false;
		};

		virtual
		void*
		Construct() const
		{
			return nullptr;
		}

		virtual
		void*
		Construct(void* a_location) const
		{
			return nullptr;
		};

		virtual
		void
		Destruct(void* a_location, bool a_deallocate = true) const { };
	};

	template<typename T>
	struct Constructor final : GenericConstructor
	{
		using value_type = T;

		constexpr static int value_size = sizeof(T);

		Constructor()
		{
			// valid is only set to true if GenericConstructor has been overridden
			valid = true;
		}

		~Constructor() override = default;

		void*
		Construct() const override
		{
			return new value_type;
		}

		void*
		Construct(void* a_location) const override
		{
			return new(a_location) value_type;
		}

		// TODO: Come up with a better way (preferably compile time) to evaluate lifetime
		void
		Destruct(void* a_location, bool a_deallocate = true) const override
		{
			value_type* p = static_cast<value_type*>(a_location);
			p->~value_type();

			if (a_deallocate)
			{
				::operator delete(a_location);
			}
		}
	};
}
