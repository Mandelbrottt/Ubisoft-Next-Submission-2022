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

	struct Field
	{
		const std::string debugName;
		const std::string fieldName;
		const std::string fieldDescription;

		const uint32_t offset;
		const uint32_t size;

		const type_info& fieldType;
		const type_info& containingType;

		const FieldFlags flags = FfNone;

		template<typename TContaining
			/*, std::enable_if_t<!std::is_pointer_v<std::remove_pointer_t<TContaining>>, bool> = true*/>
		const void* GetValue(const TContaining& a_obj) const
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

			return reinterpret_cast<void*>(ptrToMember);
		}

		template<typename TField, typename TContaining>
		const TField& GetValue(const TContaining& a_obj) const
		{
			const type_info& incomingType = typeid(TField);
			assert(incomingType == this->fieldType);

			const void* result = Field::GetValue(a_obj);

			return *static_cast<const TField*>(result);
		}

		template<typename TContaining
			/*, std::enable_if_t<!std::is_pointer_v<std::remove_pointer_t<TContaining>>, bool> = true*/>
		void SetValue(const TContaining& a_obj, const void* a_incomingValue, size_t a_incomingSize) const
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

		template<typename TContaining, typename TField>
		void SetValue(const TContaining& a_obj, const TField& a_value) const
		{
			const type_info& incomingType = typeid(TField);
			assert(incomingType == this->fieldType);

			Field::SetValue(a_obj, &a_value, sizeof(TField));
		}
	};
}
