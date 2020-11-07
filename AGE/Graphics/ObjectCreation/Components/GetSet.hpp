#pragma once

namespace age::util {

template<typename T>
class GetSet {
    T m_data;
public:
    inline const T& get() const { return m_data; }
    inline T& get() { return m_data; }
    inline void set(const T& data) { m_data = data; }
};

} // namespace age::util
