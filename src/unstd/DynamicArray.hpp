#pragma once

#include "unstd/types.hpp"
#include "unstd/unstdlib.hpp"

// Simple buffer that can grow in size and be accessed with operator[].
// It is really only meant to be used in other data structures.
// It is assumed that all elements with index < _size have been initialized.
template <typename T>
struct DynamicArray : NonCopyable {
    char* data = (char*) malloc(sizeof(T) * 2);
    u64 capacity = 2;
    u64 size = 0;

    T& operator[](u64 idx);
    const T& operator[](u64 idx) const;

    ~DynamicArray();
};

template <typename T>
inline u64 size(const DynamicArray<T>& arr) {
    return arr.size;
}

template <typename T>
void clear(DynamicArray<T>& arr)
{
    T* data_cast = reinterpret_cast<T*>(arr.data);
    for (u64 idx = 0; idx < arr.size; idx++) {
        data_cast[idx].~T();
    }

    arr.size = 0;
}

template <typename T>
void swap(DynamicArray<T>& bufferA, DynamicArray<T>& bufferB) {
    swap(bufferA.data, bufferB.data);
    swap(bufferA.capacity, bufferB.capacity);
    swap(bufferA.size, bufferB.size);
};

template <typename T>
T& DynamicArray<T>::operator[](u64 idx)
{
    assert(idx <= this->size);
    return reinterpret_cast<T*>(this->data)[idx];
}

template <typename T>
const T& DynamicArray<T>::operator[](u64 idx) const
{
    assert(idx <= this->size);
    const T* const cast_data = reinterpret_cast<const T* const>(this->data);
    return cast_data[idx];
}

template <typename T>
DynamicArray<T>::~DynamicArray(void)
{
    T* data_cast = reinterpret_cast<T*>(this->data);
    for (u64 idx = 0; idx < this->size; idx++) {
        data_cast[idx].~T();
    }

    free(this->data);
    this->data = nullptr;
    this->capacity = 0;
    this->size = 0;
}

template <typename T>
void reserve(DynamicArray<T>& self, u64 new_capacity) {
    if (new_capacity > self.capacity) {
        char* new_data = (char*) malloc(sizeof(T) * new_capacity);

        T* new_data_cast = reinterpret_cast<T*>(new_data);
        T* old_data_cast = reinterpret_cast<T*>(self.data);

        for (u64 idx = 0; idx < self.size; idx++) {
            new (&(new_data_cast[idx])) T(std::move(old_data_cast[idx]));
            old_data_cast[idx].~T();
        }

        free(self.data);
        self.data = new_data;
        self.capacity = new_capacity;
    }
}

// accepts any sort of reference/copy of T, or T constructor arguments
// to build the object in place and avoid any unnecessary copies
template <typename T, class... Args>
T& append(DynamicArray<T>& buffer, Args&&... args) {
    if (buffer.size == buffer.capacity) {
        reserve(buffer, 2 * buffer.capacity);
    }

    T* data_cast = reinterpret_cast<T*>(buffer.data);
    new (&(data_cast[buffer.size])) T(args...);
    return data_cast[buffer.size++];
}

template <typename T, typename Callable>
inline void for_each(DynamicArray<T>& buffer, Callable&& f) {
    for (auto i = 0; i < buffer.size; i++) {
        f(buffer[i]);
    }
}

