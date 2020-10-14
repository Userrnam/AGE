#pragma once

#include "TemplateUtils.hpp"

namespace age {

// Warning: does not call constructor/destructors
template<typename... Args>
class Tuple {
	uint8_t m_data[detail::TypeSizeSum<Args...>::value];
public:

    Tuple() {
        static_assert(detail::TypeRepeatInTypeArray<Args...>::value == 0);
    }

	template<typename T>
	T& get() {
        static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *reinterpret_cast<T*>(m_data + detail::TypeOffset<T, Args...>::value);
	}
};

} // namespace age
