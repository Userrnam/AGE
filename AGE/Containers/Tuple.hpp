#pragma once

#include "TemplateUtils.hpp"

namespace age {

// Warning: this calls only constructors, so arguments of the Tuple must only have simple
// constructors (default values).
template<typename... Args>
class Tuple {
	uint8_t m_data[detail::TypeSizeSum<Args...>::value];

	// constructor caller
	template<typename... Types>
	struct ConstructorCaller {};

	template<typename T, typename... Types>
	struct ConstructorCaller<T, Types...> {
		static inline void callConstructors(uint8_t* p, size_t offset) {
			new(p + offset) T();
			ConstructorCaller<Types...>::callConstructors(p, offset + sizeof(T));
		}
	};

	template<>
	struct ConstructorCaller<> {
		static inline void callConstructors(uint8_t* p, size_t offset) {}
	};

public:
    inline Tuple() {
        static_assert(detail::TypeRepeatInTypeArray<Args...>::value == 0);
		ConstructorCaller<Args...>::callConstructors(m_data, 0);
    }

	template<typename T>
	inline T& get() {
        static_assert(detail::TypeIsInTypeArray<T, Args...>::value);
		return *reinterpret_cast<T*>(m_data + detail::TypeOffset<T, Args...>::value);
	}
};

} // namespace age
