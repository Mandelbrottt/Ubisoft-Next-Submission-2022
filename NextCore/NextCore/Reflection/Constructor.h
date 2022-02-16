#pragma once

namespace NextCore::Reflection
{
	/**
	 * \brief Wrapper around construction and destruction of objects, statically evaluated
	 *        with placement new and delete through polymorphism
	 * \remark All base functions return nullptr or don't do anything instead of being pure virtual
	 *         because then we wouldn't be able to construct this on the stack.
	 */
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

		/**
		 * \brief Allocates a new object and returns it
		 * \return A pointer to the constructed object
		 */
		void*
		Construct() const override
		{
			return new value_type;
		}

		/**
		 * \brief Construct an object of type value_type in-place.
		 * \param a_location A pointer to the location to construct the object.
		 * \return The location where the object was constructed.
		 * \remark It is the caller's responsibility to ensure a_location is at least sizeof(value_type) bytes large.
		 *     <br>Currently returns the same value passed in.
		 */
		void*
		Construct(void* a_location) const override
		{
			return new(a_location) value_type;
		}

		/**
		 * \brief Destruct and deallocate the object at a_location.
		 * \param a_location A pointer to the object to destruct.
		 * \param a_deallocate Whether to deallocate the object.
		 * \remark If a_deallocate is false, or the object was constructed using
		 *         in-place \link Construct \endlink, deallocation is the caller's responsibility.
		 */
		void
		Destruct(void* a_location, bool a_deallocate = true) const override
		{
			// TODO: Come up with a better way (preferably compile time) to evaluate lifetime (Wrapper types?)

			// Call the destructor
			value_type* p = static_cast<value_type*>(a_location);
			p->~value_type();

			// De-allocate the object
			if (a_deallocate)
			{
				::operator delete(a_location);
			}
		}
	};
}
