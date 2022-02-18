#pragma once

namespace NextCore::Reflection
{
	enum FieldFlags : uint32_t
	{
		FfNone = 0,

		FfStatic = 0x01,
		//FfNonStatic = 0x02,
		FfPrivate = 0x04,
		FfProtected = 0x08,
		//FfPrivate   = 0x10,
	};

	/**
	 * \brief Runtime representation of a field member.
	 */
	struct Field
	{
		// The actual name of the field in the source code
		const std::string name;

		// The name to display to the user in visual applications. Can be user-defined.
		const std::string displayName;

		// The description of the field. Optionally user-defined, else an empty string.
		const std::string description;

		// The offset in bytes into the containing object that the field resides at.
		const uint32_t offset;

		// The size returned by sizeof of the field.
		const uint32_t size;

		// The type_info of the field.
		type_info const& fieldType;

		// The type_info of the object that the field is contained in.
		type_info const& containingType;

		// Flags that provide additional information about the field.
		const FieldFlags flags = FfNone;
		
		/**
		 * \brief Get a read-only pointer to the field for the given object.
		 * \tparam TContaining The type that contains the field (can be void*).
		 * \param a_obj The object to get the value for.
		 * \return A pointer to the instance of the field on object obj.
		 */
		template<typename TContaining
			/*, std::enable_if_t<!std::is_pointer_v<std::remove_pointer_t<TContaining>>, bool> = true*/>
		const void* GetValue(TContaining const& a_obj) const
		{
			const type_info& incomingType = typeid(std::remove_pointer_t<std::remove_cv_t<TContaining>>);
			assert(
				incomingType == this->containingType
				|| incomingType == typeid(void)
			);

			uintptr_t ptrToMember;

			if constexpr (std::is_pointer_v<TContaining>)
				ptrToMember = reinterpret_cast<uintptr_t>(a_obj);
			else
				ptrToMember = reinterpret_cast<uintptr_t>(&a_obj);

			ptrToMember += offset;

			return std::launder(reinterpret_cast<void*>(ptrToMember));
		}
		
		/**
		 * \brief Get a read-only reference to the field for the given object.
		 * \tparam TField The type of the field.
		 * \tparam TContaining The type that contains the field (can be void*).
		 * \param a_obj The object to get the value for.
		 * \return A pointer to the instance of the field on object obj.
		 */
		template<typename TContaining, typename TField>
		TField const& GetValue(TContaining const& a_obj) const
		{
			const type_info& incomingType = typeid(TField);
			assert(incomingType == this->fieldType);

			const void* result = Field::GetValue(a_obj);

			return *static_cast<const TField*>(result);
		}
		
		/**
		 * \brief Set the value for the field for the given object
		 * \tparam TContaining The type that contains the field (can be void*)
		 * \param a_obj The object to set the value for
		 * \param a_incomingValue A pointer to the value to be copied.
		 * \param a_incomingSize the size in bytes of the incoming value.
		 * \remark Uses memcpy under the hood, so avoid using with non-trivially copyable types.
		 *         This is the caller's responsibility.
		 */
		template<typename TContaining
			/*, std::enable_if_t<!std::is_pointer_v<std::remove_pointer_t<TContaining>>, bool> = true*/>
		void SetValue(TContaining const& a_obj, const void* a_incomingValue, size_t a_incomingSize) const
		{
			const type_info& incomingType = typeid(std::remove_pointer_t<TContaining>);
			assert(
				incomingType == this->containingType
				|| incomingType == typeid(void)
				|| incomingType == typeid(const void)
			);

			assert(a_incomingSize == size);

			uintptr_t ptrToMember;

			if constexpr (std::is_pointer_v<TContaining>)
				ptrToMember = reinterpret_cast<uintptr_t>(a_obj);
			else
				ptrToMember = reinterpret_cast<uintptr_t>(&a_obj);

			ptrToMember += offset;

			std::memcpy(reinterpret_cast<void*>(ptrToMember), a_incomingValue, a_incomingSize);
		}

		/**
		 * \brief Set the value for the field for the given object
		 * \tparam TContaining The type that contains the field (can be void*)
		 * \tparam TField The type of the field
		 * \param a_obj The object to set the value for
		 * \param a_value A reference to the value to be copied.
		 */
		template<typename TContaining, typename TField>
		void SetValue(TContaining const& a_obj, TField const& a_value) const
		{
			const type_info& incomingType = typeid(std::remove_pointer_t<TContaining>);
			assert(
				incomingType == this->containingType
				|| incomingType == typeid(void)
				|| incomingType == typeid(const void)
			);
			
			uintptr_t ptrToMember;

			if constexpr (std::is_pointer_v<TContaining>)
				ptrToMember = reinterpret_cast<uintptr_t>(a_obj);
			else
				ptrToMember = reinterpret_cast<uintptr_t>(&a_obj);

			ptrToMember += offset;
			
			TField& refToMember = *std::launder(reinterpret_cast<TField*>(ptrToMember));

			// Do this instead of calling other SetValue function to call copy constructor
			refToMember = a_value;
		}
	};
}
