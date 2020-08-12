#pragma once


namespace age {

template <typename T>
struct Conditional {
    T data;
    int condition;

    Conditional() {}
    inline Conditional(T t, int c = 0) : data(t), condition(c) {}
};

} // namespace age
