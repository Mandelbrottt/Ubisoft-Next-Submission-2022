#pragma once

#include <cassert>

#include "TypeId.h"

namespace Next::Reflection
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

		// The StaticTypeId of the field.
		TypeId const fieldTypeId;

		// The StaticTypeId of the object that the field is contained in.
		TypeId const containingTypeId;

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
			const TypeId incomingTypeId = GetTypeId<TContaining>();
			
			assert(
				incomingTypeId == this->containingTypeId
				|| incomingTypeId == GetTypeId<void>()
			);

			uintptr_t ptrToMember;

			if constexpr (std::is_pointer_v<TContaining>)
				ptrToMember = reinterpret_cast<uintptr_t>(a_obj);
			else
				ptrToMember = reinterpret_cast<uintptr_t>(&a_obj);

			ptrToMember += offset;

			return reinterpret_cast<void*>(ptrToMember);
		}
		
		/**
		 * \brief Get a read-only reference to the field for the given object.
		 * \tparam TField The type of the field.
		 * \tparam TContaining The type that contains the field (can be void*).
		 * \param a_obj The object to get the value for.
		 * \return A pointer to the instance of the field on object obj.
		 */
		template<typename TField, typename TContaining>
		TField const& GetValue(TContaining const& a_obj) const
		{
			const TypeId incomingTypeId = GetTypeId<TField>();
			assert(incomingTypeId == this->fieldTypeId);

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
			const TypeId incomingTypeId = GetTypeId<TContaining>();
			assert(
				incomingTypeId == this->containingTypeId
				|| incomingTypeId == GetTypeId<void>()
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
			const TypeId incomingTypeId = GetTypeId<TContaining>();
			assert(
				incomingTypeId == this->containingTypeId
				|| incomingTypeId == GetTypeId<void>()
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
	
	/**
	 * \brief The display name of a reflected member
	 */
	struct Name
	{
		constexpr
		Name()
			: c_str("") {}

		constexpr
		Name(const char* a_value)
			: c_str(a_value) {}

		const char* c_str;
	};

	/**
	 * \brief The description of a reflected member
	 */
	struct Description
	{
		constexpr
		Description()
			: c_str("") {}

		constexpr
		Description(const char* a_value)
			: c_str(a_value) {}

		const char* c_str;
	};
}
