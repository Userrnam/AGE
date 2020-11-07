#pragma once

#include <initializer_list>

#include "DefaultAllocator.hpp"

namespace age {

// warining: do not do stuff like this:
// array1 = array1 + smth
// this will create a memory leak
// use += in such situations

template<typename T, typename Allocator=DefaultAllocator>
class DynamicArray {
protected:
	T* m_data = nullptr;
	size_t m_index = 0;
	size_t m_allocated = 0;

public:
	DynamicArray() {}
	DynamicArray(size_t count) { this->create(count); }
	DynamicArray(std::initializer_list<T> l) { this->create(l); }

	void create(size_t count = 16);
	void create(std::initializer_list<T> l);

	DynamicArray copy() const;

	void destroy();

	// getters

	T* data() { return m_data; }
	const T* data() const { return m_data; }

	T& operator[](size_t i) { return m_data[i]; }
	const T& operator[](size_t i) const { return m_data[i]; }

	T& back() { return m_data[m_index-1]; }
	const T& back() const { return m_data[m_index-1]; }

	size_t count() const { return m_index; }
	size_t allocated() const { return m_allocated; }

	// modifiers

	void clear() { m_index = 0; }

	void resize(size_t count);
	void reserve(size_t count);
	void shrink();

	void add(const T& val);
	void erase(size_t first, size_t last);

	// operators

	DynamicArray operator+(const DynamicArray& rhs) const;
	DynamicArray& operator+=(const DynamicArray& rhs);

	DynamicArray operator+(std::initializer_list<T> l) const;
	DynamicArray& operator+=(std::initializer_list<T> l);
	DynamicArray& operator=(std::initializer_list<T> l);
};

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::create(size_t count) {
	m_allocated = count;
	m_data = (T*)Allocator::allocData(sizeof(T) * count);
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::create(std::initializer_list<T> l) {
	this->create(l.size());

	m_index = l.size();

	auto p = l.begin();
	for (size_t i = 0; i < l.size(); ++i) {
		m_data[i] = *p;
		p++;
	}
}

template<typename T, typename Allocator>
DynamicArray<T, Allocator> DynamicArray<T, Allocator>::copy() const {
	DynamicArray result;

	result.m_allocated = m_allocated;
	result.m_index = m_index;
	result.m_data = (T*)Allocator::allocData(sizeof(T) * m_allocated);

	memcpy(result.m_data, m_data, sizeof(T) * m_allocated);

	return result;
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::destroy() {
	Allocator::freeData(m_data);
	m_data = nullptr;
}

/* modifiers */

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::resize(size_t count) {
	m_index = count - 1;
	m_allocated = count;

	m_data = (T*)Allocator::reallocData(m_data, sizeof(T) * m_allocated);
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::reserve(size_t count) {
	if (m_allocated >= count) {
		return;
	}

	m_allocated = count;

	m_data = (T*)Allocator::reallocData(m_data, sizeof(T) * m_allocated);
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::shrink() {
	m_allocated = m_index;
	m_data = (T*)Allocator::reallocData(m_data, sizeof(T) * m_allocated);
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::add(const T& val) {
	if (m_index == m_allocated) {
		m_allocated *= 2;
		m_data = (T*)Allocator::reallocData(m_data, sizeof(T) * m_allocated);
	}

	m_data[m_index] = val;
	
	m_index++;
}

template<typename T, typename Allocator>
void DynamicArray<T, Allocator>::erase(size_t first, size_t last) {
	memmove(m_data + first, m_data + last, m_index - last);
	m_index -= (last - first);
}

// operators

template<typename T, typename Allocator>
DynamicArray<T, Allocator> DynamicArray<T, Allocator>::operator+(const DynamicArray& rhs) const {
	DynamicArray<T> result;
	result.create(this->count() + rhs.count());
	result.m_index = result.allocated();

	memcpy(result.m_data, m_data, this->count() * sizeof(T));
	memcpy(result.m_data + this->count(), rhs.m_data, rhs.count() * sizeof(T));

	return result;
}

template<typename T, typename Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::operator+=(const DynamicArray& rhs) {
	size_t index = m_index;

	m_index += rhs.m_index;

	if (m_index < m_allocated) {
		resize(m_index);
	}

	for (size_t i = 0; index < m_index; ++index, ++i) {
		m_data[index] = rhs.m_data[i];
	}

	return *this;
}

template<typename T, typename Allocator>
DynamicArray<T, Allocator> DynamicArray<T, Allocator>::operator+(std::initializer_list<T> l) const {
	DynamicArray<T> result = this->copy();
	result += l;
	return result;
}

template<typename T, typename Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::operator+=(std::initializer_list<T> l) {
	reserve(this->count() + l.size());
	for (auto& e : l) {
		this->add(e);
	}
	return *this;
}

template<typename T, typename Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::operator=(std::initializer_list<T> l) {
	clear();
	reserve(l.size());
	for (auto& e : l) {
		this->add(e);
	}

	return *this;
}

} // namespace age
