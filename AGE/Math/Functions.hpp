#pragma once


namespace age {

template<typename T>
inline T lerp(const T& start, const T& end, float progress) {
    return start + progress * (end - start);
}

} // namespace age
