#pragma once

#include "TemplateUtils.hpp"


namespace age {

// Warning: does not call constructor/destructors
template<typename... Args>
class TaggedUnion {
	uint8_t m_data[detail::MaxTypeLen<Args...>::value];
	size_t tag = 0;

public:
    TaggedUnion() {}

    template<typename T>
    TaggedUnion(const T& other) {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);

		tag = detail::TypeIndex<T, Args...>::value;
		memcpy(m_data, &other, sizeof(T)); 
    }

	template<typename T>
	const TaggedUnion& operator=(const T& other) {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);

		tag = detail::TypeIndex<T, Args...>::value;
		memcpy(m_data, &other, sizeof(T));

		return *this;
	}

    template<typename T>
    const T& get() const {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *(T*)m_data;
    }

    template<typename T>
    T& get() {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *(T*)m_data;
    }

	template<typename T>
	operator T() const {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *(T*)m_data;
	}

	template<typename T>
	operator T&() {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *(T*)m_data;
	}

	template<typename T>
	bool is() const {
		static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return tag == detail::TypeIndex<T, Args...>::value;
	}
};

} // namespace age
