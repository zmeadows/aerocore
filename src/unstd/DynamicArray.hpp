#pragma once

#include "unstd/unstdlib.hpp"
#include <assert.h>

#define DEBUG_ARRAY(x) \
    (std::cout << "[" << __FILE__ << "::" << __LINE__ << "] :: " << x << std::endl)
// #define DEBUG_ARRAY(x)

template <typename T>
struct DynamicArray {
    u8* data = nullptr;
    u64 capacity = 0;
    u64 size = 0;

    inline T& operator[](u64 idx);
    inline const T& operator[](u64 idx) const;

    T* begin(void) { return reinterpret_cast<T*>(this->data); }
    T* end(void) { return reinterpret_cast<T*>(this->data) + this->size; }
    const T* const begin(void) const { return reinterpret_cast<const T* const>(this->data); }
    const T* const end(void) const { return reinterpret_cast<const T* const>(this->data) + this->size; }
};

template <typename T>
void debug_print(const DynamicArray<T>* self) {
    for (const T& x : *self) {
        std::cout << x << " ";
    }
    std::cout << " :: size = " << self->size << " :: capacity = " << self->capacity << " :: data @ "
              << static_cast<void*>(self->data) << std::endl;
}

template <typename T>
static void deallocate(DynamicArray<T>* self) {
    free(self->data);
    self->capacity = 0;
    self->size = 0;
}

template <typename T>
void reserve_memory(DynamicArray<T>* self, u64 new_capacity) {
    if (new_capacity > self->capacity) {
        u8* new_data = memalloc<u8, T>(new_capacity);

        T* new_data_cast = reinterpret_cast<T*>(new_data);
        T* old_data_cast = reinterpret_cast<T*>(self->data);

        for (u64 idx = 0; idx < self->size; idx++) {
            new (&(new_data_cast[idx])) T(old_data_cast[idx]);
        }

        free(old_data_cast);
        self->data = new_data;
        self->capacity = new_capacity;
    }
}

template <typename T>
u64 size(const DynamicArray<T>* self) {
    return self->size;
}

template <typename T>
void clear(DynamicArray<T>* self) {
    self->size = 0;
}

template <typename T>
inline T& DynamicArray<T>::operator[](u64 idx) {
    assert(idx < this->size);
    T* cast_data = reinterpret_cast<T*>(this->data);
    return cast_data[idx];
}

template <typename T>
inline const T& DynamicArray<T>::operator[](u64 idx) const {
    assert(idx < this->size);
    const T* const cast_data = reinterpret_cast<const T* const>(this->data);
    return cast_data[idx];
}

template <typename T>
DynamicArray<T> copy(const DynamicArray<T>* other) {
    DynamicArray<T> new_array;
    reserve_memory(new_array, other.capacity);

    T* const other_data_cast = reinterpret_cast<T*>(other.data);
    T* this_data_cast = reinterpret_cast<T*>(self->data);
    for (auto i = 0; i < other.size; i++) {
        new (&(this_data_cast[i])) T(other_data_cast[i]);
    }

    new_array.size = other.size;

    return new_array;
}

template <typename T>
bool contains(const DynamicArray<T>* self, const T& value) {
    for (const T& x : self) {
        if (x == value)
            return true;
    }
    return false;
}

template <typename T>
void insert_at(DynamicArray<T>* self, const T& value, u64 idx) {
    DEBUG_ARRAY("Insert value " << value << " at index " << idx);
    assert(idx <= self->size);

    if (self->size == self->capacity) {
        reserve_memory(self, self->capacity == 0 ? 2 : 2 * self->capacity);
    }

    self->size++;

    for (u64 i = self->size - 1; i > idx; i--) {
        (*self)[i] = (*self)[i - 1];
    }

    T* data_cast = reinterpret_cast<T*>(self->data);
    new (&(data_cast[idx])) T(value);
    DEBUG_ARRAY("Finished insert.");
}

template <typename T>
void remove_at(DynamicArray<T>* self, u64 idx) {
    assert(idx < self->size);

    for (u64 i = idx; i < self->size - 1; i++) {
        (*self)[i] = (*self)[i + 1];
    }

    self->size--;
}

template <typename T, class... Args>
void append(DynamicArray<T>* self, Args&&... args) {
    if (self->size == self->capacity) {
        reserve_memory(self, max((u64)2, 2 * self->capacity));
    }

    T* data_cast = reinterpret_cast<T*>(self->data);
    new (&(data_cast[self->size])) T(args...);
    self->size++;
}

template <typename T>
T& back(DynamicArray<T>* self) {
    return self[self->size - 1];
}
