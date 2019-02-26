#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/unstdlib.hpp"
#include "unstd/Maybe.hpp"
#include "unstd/Slice.hpp"

template <typename T>
class ArraySet {
    using Self = ArraySet<T>;

    DynamicArray<T> arr;

    struct BinarySearchResult {
        enum class Type {
            End,
            InsertIndex,
            AlreadyExists
        } type;

        s64 index = -1;
    };

    BinarySearchResult binary_search(const T& item) const {
        const Self& self = *this;

        BinarySearchResult result;

        if (self.arr.size() == 0) {
                result.type = BinarySearchResult::Type::End;
                return result;
        }

        if (item > self.arr.back()) {
            result.type = BinarySearchResult::Type::End;
            return result;
        }

        s64 low = 0;
        s64 high = self.arr.size() - 1;

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

public:

    T* begin(void) { return arr.begin(); }
    T* end(void) { return arr.end(); }
    const T* const begin(void) const { return arr.begin(); }
    const T* const end(void) const { return arr.end(); }

    DynamicArray<Slice<T>> split(u64 n) const {
        const Self& self = *this;

        auto slices = DynamicArray<Slice<T>>::init(n);

        const u64 q = std::max(self.size() / n, (u64) 1);

        for (u64 i = 0; i < n && i < self.size(); i++) {
            const T* const slice_start = self.arr.raw() + i * q;
            size_t slice_end;
            if (i == n-1) {
                slice_end = (size_t) (this->end() - slice_start);
            } else {
                slice_end = std::min((size_t)(this->end() - self.arr.raw()) - i*q, q);
            }

            slices.append(slice_start, slice_end);
        }

        return slices;
    }

    inline void reserve(u64 new_capacity) {
        this->arr.reserve(new_capacity);
    }

    inline u64 size(void) const {
        return this->arr.size();
    }

    inline void clear(void) { this->arr.clear(); }

    inline void copy_from(const Self& other) {
        this->arr.copy_from(other.arr);
    }

    void insert(const T& new_value) {
        Self& self = *this;

        self.arr.grow();

        const BinarySearchResult result = self.binary_search(new_value);

        switch (result.type) {
            case BinarySearchResult::Type::End:
                self.arr.append(new_value);
                break;
            case BinarySearchResult::Type::InsertIndex:
                self.arr.insert_at(new_value, result.index);
                break;
            case BinarySearchResult::Type::AlreadyExists:
                break;
        }
    }

    void remove(const T& value) {
        Self& self = *this;

        const BinarySearchResult result = self.binary_search(value);

        if (result.type == BinarySearchResult::Type::AlreadyExists) {
            self.arr.remove_at(result.index);
        }
    }

    bool contains(const T& value) const {
        return this->binary_search(value).type == BinarySearchResult::Type::AlreadyExists;
    }
};

