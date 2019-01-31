#pragma once

#include <iostream>

#include "unstd/DynamicArray.hpp"
#include "unstd/types.hpp"
#include "unstd/unstdlib.hpp"

struct BucketBase {
    u8* data;
    bool* occupied;
    u16 filled_slots;
    u16 capacity;

    static void destroy(BucketBase* self) {
        free(self->data); self->data = nullptr;
        free(self->occupied); self->occupied = nullptr;
        self->filled_slots = 0;
        self->capacity = 0;
    }
};

template <typename T>
struct Bucket : BucketBase {
    T& operator[](u16 idx);
    const T& operator[](u16 idx) const;

    // this function is the only way one should ever create a Bucket
    static Bucket<T>* allocate(u64 bucket_capacity) {
        assert(bucket_capacity < 65000);
        Bucket<T>* new_bucket = memalloc<Bucket<T>>(1);

        new_bucket->data         = memalloc<u8, T>(bucket_capacity);
        new_bucket->occupied     = memalloc<bool>(bucket_capacity);
        new_bucket->filled_slots = 0;
        new_bucket->capacity     = bucket_capacity;

        for (auto i = 0; i < new_bucket->capacity; i++) {
            new_bucket->occupied[i] = false;
        }

        return new_bucket;
    }
};

void release_slot(BucketBase* self, u32 slot_idx);
u16 find_open_slot(BucketBase* self);
bool is_full(BucketBase* self);

template <typename T>
T& Bucket<T>::operator[](u16 idx) {
    assert(idx < this->capacity && this->occupied[idx]);
    T* cast_data = reinterpret_cast<T*>(this->data);
    return cast_data[idx];
}

template <typename T>
const T& Bucket<T>::operator[](u16 idx) const {
    assert(idx < this->capacity && this->occupied[idx]);
    const T* const cast_data = reinterpret_cast<const T* const>(this->data);
    return cast_data[idx];
}

template <typename T, typename... Args>
u16 insert_in_bucket(Bucket<T>* self, Args&&... args) {
    const u16 new_slot_idx = find_open_slot(self);
    assert(new_slot_idx < self->capacity);
    T* cast_data = reinterpret_cast<T*>(self->data);
    new ( &(cast_data[new_slot_idx]) ) T(args...);
    self->occupied[new_slot_idx] = true;
    self->filled_slots++;
    return new_slot_idx;
}

struct BucketIndex {
    u16 bucket;
    u16 slot;
};

std::ostream& operator<<(std::ostream& os, const BucketIndex index);

struct BucketArrayBase {
    u64 bucket_capacity = 64;
    DynamicArray<bool> bucket_filled;
    DynamicArray<BucketBase*> buckets;

    static void destroy(BucketArrayBase* self) {
        for (BucketBase* bucket : self->buckets)
            BucketBase::destroy(bucket);

        deallocate(&self->buckets);
        deallocate(&self->bucket_filled);
    }
};

u64 size(const BucketArrayBase* self);
void remove_item(BucketArrayBase* self, const BucketIndex index);

template <typename T>
struct BucketArray : BucketArrayBase {
    static BucketArray<T> create(u16 bucket_size) {
        BucketArray<T> new_array;
        new_array.bucket_capacity = bucket_size;

        new_array.buckets = DynamicArray<BucketBase*>();
        reserve_memory(&new_array.buckets, 4);

        new_array.bucket_filled = DynamicArray<bool>();
        reserve_memory(&new_array.bucket_filled, 4);

        return new_array;
    }

    T& operator[](BucketIndex index);
};


template <typename T, typename... Args>
const BucketIndex insert(BucketArray<T>* self, Args&&... args) {
    for (u16 bucket_id = 0; bucket_id < self->buckets.size; bucket_id++) {
        Bucket<T>* bucket = static_cast<Bucket<T>*>(self->buckets[bucket_id]);

        if (!is_full(bucket)) {
            u16 slot_id = insert_in_bucket(bucket, args...);
            return BucketIndex{bucket_id, slot_id};
        }
    }

    // If we reach here, we couldn't find an empty slot in the old buckets.
    // So make a new bucket.
    auto bucket = Bucket<T>::allocate(self->bucket_capacity);
    insert_in_bucket(bucket, args...);
    append(&self->buckets, bucket);
    return BucketIndex { (u16) (self->buckets.size - 1), 0};
}

template <typename T>
inline T& BucketArray<T>::operator[](const BucketIndex index) {
    assert(index.bucket < this->buckets.size && "BucketIndex points to invalid bucket!");
    Bucket<T>* bucket = static_cast<Bucket<T>*>(this->buckets[index.bucket]);
    return (*bucket)[index.slot];
}
