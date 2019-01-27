#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/unstdlib.hpp"
#include "unstd/Maybe.hpp"

template <typename T>
struct ArraySet {
    DynamicArray<T> arr;

    T* begin(void) { return arr.begin(); }
    T* end(void) { return arr.end(); }
    const T* const begin(void) const { return arr.begin(); }
    const T* const end(void) const { return arr.end(); }
};

namespace {

struct BinarySearchResult {
    enum class Type {
        End,
        InsertIndex,
        AlreadyExists
    } type;

    s64 index = -1;
};

template <typename T>
void debug_print(const ArraySet<T>& self) {
    debug_print(self.arr);
}

template <typename T>
BinarySearchResult binary_search(const ArraySet<T>& self, const T& item) {
    BinarySearchResult result;

    if (self.arr.size == 0) {
            result.type = BinarySearchResult::Type::End;
            return result;
    }

    if (item > self.arr[self.arr.size-1]) {
        result.type = BinarySearchResult::Type::End;
        return result;
    }

    s64 low = 0;
    s64 high = self.arr.size - 1;

    do {
        u64 mid = low + (high - low) / 2;
        const T& mid_value = self.arr[mid];

        if (item == mid_value) {
            result.type = BinarySearchResult::Type::AlreadyExists;
            result.index = mid;
            return result;
        } else if (item < mid_value) {
            high = mid - 1;
        } else { // value > self.arr[mid]
            low = mid + 1;
        }
    } while (high >= low);

    result.type = BinarySearchResult::Type::InsertIndex;
    result.index = low;

    return result;
}

}

template <typename T>
void reserve_memory(ArraySet<T>& self, u64 new_capacity) {
    reserve_memory(self.arr, new_capacity);
}

template <typename T>
u64 size(const ArraySet<T>& self) {
    return self.arr.size;
}

template <typename T>
void clear(ArraySet<T>& self) { clear(self.arr); }

template <typename T>
ArraySet<T> copy(const ArraySet<T>& other) {
    ArraySet<T> new_set;
    new_set.arr = copy(other.arr);
    return new_set;
}

template <typename T>
void insert(ArraySet<T>& self, const T& new_value) {
    if (self.arr.size == self.arr.capacity) {
        reserve_memory(self.arr, self.arr.capacity == 0 ? 2 : 2 * self.arr.capacity);
    }

    const BinarySearchResult result = binary_search(self, new_value);

    switch (result.type) {
        case BinarySearchResult::Type::End:
            append(self.arr, new_value);
            break;
        case BinarySearchResult::Type::InsertIndex:
            insert_at(self.arr, new_value, result.index);
            break;
        case BinarySearchResult::Type::AlreadyExists:
            break;
    }
}

template <typename T>
void remove(ArraySet<T>& self, const T& value) {
    const BinarySearchResult result = binary_search(self, value);

    if (result.type != BinarySearchResult::Type::AlreadyExists)
        return;

    remove_at(self.arr, result.index);
}

//TODO: This can be optimized slightly by creating a modified version of the binary_search function.
template <typename T>
bool contains(const ArraySet<T>& self, const T& value) {
    return binary_search(self, value).type == BinarySearchResult::Type::AlreadyExists;
}

// void test_ArraySet(void) {
//     ArraySet<int> test;
//     reserve_memory(test, 1);
//     debug_print(test);
// 
//     insert(test, 1);
//     debug_print(test);
//     remove(test, 1);
//     debug_print(test);
//     insert(test, 1);
//     debug_print(test);
//     insert(test, 2);
//     debug_print(test);
//     insert(test, 3);
//     debug_print(test);
//     insert(test, 4);
//     debug_print(test);
//     insert(test, -1);
//     debug_print(test);
//     insert(test, 10);
//     debug_print(test);
//     insert(test, 6);
//     debug_print(test);
//     remove(test, 1653456765);
//     debug_print(test);
//     remove(test, 2);
//     debug_print(test);
//     insert(test, 2);
//     debug_print(test);
// }
