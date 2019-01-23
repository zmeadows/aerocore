#pragma once

template <typename T>
struct ArraySet : NonCopyable {
    DynamicArray<T> arr;
};

template <typename T>
void reserve(ArraySet<T>& self, u32 new_capacity) {
    reserve(self.arr, new_capacity);
}

template <typename T>
inline u32 size(const ArraySet<T>& self) {
    return self.arr.size;
}

template <typename T>
inline void clear(ArraySet<T>& self) { clear(self.arr); }

template <typename T>
u32 _upper_bound(const ArraySet<T>& self, const T& value) {
    if (self.arr.size == 0 || value <= self.arr[0]) {
        return 0;
    } else if (value > self.arr[self.arr.size - 1]) {
        return self.arr.size;
    }

    u32 low = 0;
    u32 high = self.arr.size;

    while (high - low > 1) {
        u32 mid = low + (high - low) / 2;

        if (self.arr[mid] == value) {
            return mid;
        } else if (value < self.arr[mid]) {
            high = mid;
        } else { // value > self.arr[mid]
            low = mid;
        }
    }

    return high;
}

template <typename T>
void insert(ArraySet<T>& self, const T& new_value) {
    if (self.arr.size == self.arr.capacity) {
        reserve(self.arr, 2 * self.arr.capacity);
    }

    u32 new_idx = _upper_bound(self, new_value);

    if (new_value == self.arr[new_idx])
		return;

    // copy the old elements one step forward
    for (u32 idx = self.arr.size; idx > new_idx; idx--) {
        self.arr[idx] = self.arr[idx - 1];
    }

    self.arr[new_idx] = new_value;

    self.arr.size++;
}

template <typename T>
void remove(ArraySet<T>& self, const T& value) {
    u32 old_idx = _upper_bound(self, value);

    if (value != self.arr[old_idx])
        return;

    // copy the old elements one step backward
    for (u32 i = old_idx; i < self.arr.size - 1; i++) {
        self.arr[i] = self.arr[i + 1];
    }

    self.arr.size--;
}

template <typename T>
bool contains(ArraySet<T>& self, const T& value) {
    const u32 idx = _upper_bound(self, value);
    return (idx < self.arr.size && value == self.arr[idx]);
}

template <typename T, typename Callable>
inline void for_each(ArraySet<T>& aset, Callable&& f) {
    for_each(aset.arr, f);
}
