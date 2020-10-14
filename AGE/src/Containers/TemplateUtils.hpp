#pragma once


namespace age::detail {

// TypeIndex
template<typename Target, typename Second, typename... Last>
struct TypeIndex {
    static const size_t value = TypeIndex<Target, Last...>::value + 1;
};

template<typename Target, typename... Last>
struct TypeIndex<Target, Target, Last...> {
    static const size_t value = 1;
};

// MaxTypeLen
template<typename First, typename... Last>
struct MaxTypeLen {
    static const size_t value = sizeof(
        typename std::conditional<
            sizeof(First) >= MaxTypeLen<Last...>::value,
            First,
            uint8_t[MaxTypeLen<Last...>::value]
        >::type
    );
};

template<typename First>
struct MaxTypeLen<First> {
    static const size_t value = sizeof(First);
};

// TypeIsInTypeArray
template<typename...>
struct TypeIsInTypeArray;

template<typename T, typename Y, typename... Ts>
struct TypeIsInTypeArray<T, Y, Ts...> {
    static const bool value = sizeof(
        typename std::conditional<
            TypeIsInTypeArray<T, Ts...>::value,
            uint8_t,
            uint8_t[0]
        >::type
    );
};

template<typename T, typename... Ts>
struct TypeIsInTypeArray<T, T, Ts...> {
    static const bool value = true;
};

template<typename T>
struct TypeIsInTypeArray<T> {
    static const bool value = false;
};

// TypeSizeSum
template<typename First, typename... Last>
struct TypeSizeSum {
    static const size_t value = sizeof(First) + TypeSizeSum<Last...>::value;
};

template<typename T>
struct TypeSizeSum<T> {
    static const size_t value = sizeof(T);
};

// TypeOffset
template<typename T, typename First, typename... Last>
struct TypeOffset {
    static const size_t value = sizeof(First) + TypeOffset<T, Last...>::value;
};

template<typename T, typename... Last>
struct TypeOffset<T, T, Last...> {
    static const size_t value = 0;
};

// TypeInTypeArrayCount
template<typename T, typename... Args>
struct TypeInTypeArrayCount;

template<typename T, typename First, typename... Last>
struct TypeInTypeArrayCount<T, First, Last...> {
    static const size_t value = TypeInTypeArrayCount<T, Last...>::value;
};

template<typename T, typename... Last>
struct TypeInTypeArrayCount<T, T, Last...> {
    static const size_t value = 1 + TypeInTypeArrayCount<T, Last...>::value;
};

template<typename T>
struct TypeInTypeArrayCount<T> {
    static const size_t value = 0;
};

// TypeRepeatInTypeArray
template<typename... Args>
struct TypeRepeatInTypeArray;

template<typename First, typename... Last>
struct TypeRepeatInTypeArray<First, Last...> {
	static const size_t value = TypeInTypeArrayCount<First, Last...>::value + TypeRepeatInTypeArray<Last...>::value;
};

template<typename T>
struct TypeRepeatInTypeArray<T> {
	static const size_t value = 0;
};

} // namespace age::detail
