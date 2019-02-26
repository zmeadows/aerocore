#pragma once

#include "unstd/unstdlib.hpp"
#include <assert.h>

// TODO: add stack storage
template <typename T>
class DynamicArray {
    using Self = DynamicArray<T>;

    u8* data = nullptr;
    u64 capacity = 0;
    u64 count = 0;

public:
    T* raw(void) {
        return reinterpret_cast<T*>(this->data);
    }

    const T* const raw(void) const {
        return reinterpret_cast<const T* const>(this->data);
    }

    void grow(void) {
        Self& self = *this;
        if (self.count < self.capacity) return;

        static const f64 GROWTH_RATIO = 1.6180339887498948482;

        if (self.capacity == 0) {
            self.reserve((u64) 2);
        } else {
            const u64 new_capacity = (u64) (GROWTH_RATIO * (f64) self.capacity);
            self.reserve(new_capacity);
        }
    }

    static Self init(u64 init_capacity) {
        Self new_array;
        new_array.reserve(init_capacity);
        return new_array;
    }

    void deallocate(void) {
        Self& self = *this;
        free(self.data);
        self.data = nullptr;
        self.capacity = 0;
        self.count = 0;
    }


    void reserve(u64 new_capacity) {
        Self& self = *this;

        if (new_capacity > self.capacity) {
            u8* new_data = memalloc<u8, T>(new_capacity);

            T* new_data_cast = reinterpret_cast<T*>(new_data);
            T* old_data_cast = reinterpret_cast<T*>(self.data);

            for (u64 idx = 0; idx < self.count; idx++) {
                new (&(new_data_cast[idx])) T(old_data_cast[idx]);
            }

            free(old_data_cast);
            self.data = new_data;
            self.capacity = new_capacity;
        }
    }

    template <class... Args>
    void append(Args&&... args) {
        Self& self = *this;

        self.grow();

        T* data_cast = reinterpret_cast<T*>(self.data);
        new (&(data_cast[self.count])) T(args...);
        self.count++;
    }

    void copy_from(const Self& other) {
        Self& self = *this;

        self.clear();
        self.reserve(other.count);
        for (const T& item : other) {
            self.append(item);
        }
    }

    inline void clear(void) {
        this->count = 0;
    }

    inline u64 size(void) const {
        return this->count;
    }

    inline T& operator[](u64 idx) {
        assert(idx < this->count);
        T* cast_data = reinterpret_cast<T*>(this->data);
        return cast_data[idx];
    }

    inline const T& operator[](u64 idx) const {
        assert(idx < this->count);
        const T* const cast_data = reinterpret_cast<const T* const>(this->data);
        return cast_data[idx];
    }

    inline bool contains(const T& value) {
        for (const T& x : *this) {
            if (x == value)
                return true;
        }
        return false;
    }

    inline u64 allocated(void) const {
        return this->capacity;
    }

    void insert_at(const T& value, u64 idx) {
        Self& self = *this;

        assert(idx <= self.count);

        self.grow();

        self.count++;

        //TODO: use memcpy
        for (u64 i = self.count - 1; i > idx; i--) {
            self[i] = self[i - 1];
        }

        T* data_cast = reinterpret_cast<T*>(self.data);
        new (&(data_cast[idx])) T(value);
    }

    void remove_at(u64 idx) {
        Self& self = *this;

        assert(idx < self.count && "DynamicArray: Invalid index passed to remove_at!");

        //TODO: use memcpy
        for (u64 i = idx; i < self.count - 1; i++) {
            self[i] = self[i + 1];
        }

        self.count--;
    }

    T& back(void) {
        Self& self = *this;
        assert(self.count > 0 && "DynamicArray: Called back() on empty array!");
        return (*this)[this->count - 1];
    }

    const T& back(void) const {
        const Self& self = *this;
        assert(self.count > 0 && "DynamicArray: Called (const) back() on empty array!");
        return (*this)[this->count - 1];
    }


    T* begin(void) { return reinterpret_cast<T*>(this->data); }
    T* end(void) { return reinterpret_cast<T*>(this->data) + this->count; }
    const T* const begin(void) const { return reinterpret_cast<const T* const>(this->data); }
    const T* const end(void) const { return reinterpret_cast<const T* const>(this->data) + this->count; }
};


