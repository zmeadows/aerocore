#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/types.hpp"
#include "unstd/unstdlib.hpp"

template <typename T>
struct Bucket {
    u8* data;
    bool* occupied;
    u16 filled_slots;
    u16 capacity;

    static void destroy(Bucket<T>& self) {
        free(self.data);
        free(self.occupied);
        self.data = nullptr;
        self.occupied = nullptr;
        self.filled_slots = 0;
    }

    static Bucket<T>* allocate(u64 bucket_capacity) {
        assert(bucket_capacity < 65000);
        Bucket<T>* new_bucket = memalloc<Bucket<T>>(1);
        new_bucket->data = memalloc<u8, T>(bucket_capacity);
        new_bucket->occupied = memalloc<bool, bool>(bucket_capacity);
        new_bucket->filled_slots = 0;
        new_bucket->capacity = bucket_capacity;

        for (auto i = 0; i < new_bucket->capacity; i++) {
            new_bucket->occupied[i] = false;
        }

        return new_bucket;
    }

    T& operator[](u16 idx);
    const T& operator[](u16 idx) const;
};


template <typename T>
T& Bucket<T>::operator[](u16 idx) {
    assert(idx < this->capacity && this->occupied[idx]);
    T* cast_data = reinterpret_cast<T*>(data);
    return cast_data[idx];
}

template <typename T>
const T& Bucket<T>::operator[](u16 idx) const {
    assert(idx < this->capacity && this->occupied[idx]);
    const T* const cast_data = reinterpret_cast<const T* const>(this->data);
    return cast_data[idx];
}

template <typename T>
void release_slot(Bucket<T>& self, u32 slot_idx) {
    assert(self.occupied[slot_idx]);
    self.occupied[idx] = false;
    self.filled_slots--;
}

template <typename T>
u16 find_open_slot(const Bucket<T>& self) {
    for (s32 i = 0; i < self.capacity; i++) {
        if (!self.occupied[i])
            return i;
    }
    return self.capacity;
}

template <typename T, typename... Args>
u16 insert_in_bucket(Bucket<T>& self, Args&&... args) {
    const u16 new_slot_idx = find_open_slot(self);
    assert(new_slot_idx < self.capacity);
    T* cast_data = reinterpret_cast<T*>(self.data);
    new ( &(cast_data[new_slot_idx]) ) T(args...);
    self.occupied[new_slot_idx] = true;
    self.filled_slots++;
    return new_slot_idx;
}

template <typename T>
bool is_full(Bucket<T>& self) {
    return self.filled_slots == self.capacity;
}

struct BucketIndex {
    u16 bucket;
    u16 slot;
};

template <typename T>
struct BucketArray {
    const u64 bucket_capacity = 64;
    DynamicArray<Bucket<T>*> buckets;
    DynamicArray<bool> bucket_filled;

    static void destroy(BucketArray<T>& self) {
        for (Bucket<T>* bucket : self.buckets)
            Bucket<T>::destroy(*bucket);
    }

    static BucketArray<T> create(u16 bucket_size) {
        return BucketArray<T>{bucket_size};
    }

    T& operator[](const BucketIndex index);
    const T& operator[](const BucketIndex index) const;
};

template <typename T>
u64 size(const BucketArray<T>& self) {
    u64 sum = 0;
    for (const Bucket<T>* bucket : self.buckets) {
        sum += (u64) bucket->filled_slots;
    }
    return sum;
}

template <typename T, typename... Args>
const BucketIndex insert(BucketArray<T>& self, Args&&... args) {
    for (u16 bucket_id = 0; bucket_id < self.buckets.size; bucket_id++) {
        Bucket<T>* bucket = self.buckets[bucket_id];

        if (!is_full(*bucket)) {
            u16 slot_id = insert_in_bucket(*bucket, args...);
            return BucketIndex{bucket_id, slot_id};
        }
    }

    // If we reach here, we couldn't find an empty slot in the old buckets.
    // So make a new bucket.
    append(self.buckets, Bucket<T>::allocate(self.bucket_capacity));
    insert_in_bucket(*(back(self.buckets)), args...);
    return BucketIndex { (u16) (self.buckets.size - 1), 0};
}

template <typename T, class... Args>
void remove(BucketArray<T>& self, const BucketIndex index) {
    release_slot(self.buckets[index.bucket], index.slot);
}

template <typename T>
inline T& BucketArray<T>::operator[](const BucketIndex index) {
    assert(index.bucket < this->buckets.size && "BucketIndex points to invalid bucket!");
    Bucket<T>& bucket = *(this->buckets[index.bucket]);
    return bucket[index.slot];
}

template <typename T>
inline const T& BucketArray<T>::operator[](const BucketIndex index) const {
    assert(index.bucket < this->buckets.size && "BucketIndex points to invalid bucket!");
    const Bucket<T>& bucket = *(this->buckets[index.bucket]);
    return bucket[index.slot];
}
