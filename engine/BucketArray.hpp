#pragma once

// Simple c-style bucket array. Does not manage lifetime of contained entities,
// as this structure is only intended to store data in contiguous blocks

struct BucketIndex {
    const u32 bucket;
    const u32 index;
};

struct Bucket {
    bool* occupied;
    void* data;
    u32 alive_count;
    const u32 capacity;
};

struct BucketArray {
    Bucket* buckets;
    u32 bucket_count;
    const u32 items_per_bucket;
    const u32 item_size_bytes;

    BucketArray(const u32 bucket_size, const u32 item_size) : items_per_bucket(
};

template <typename T>
void* get(BucketArray* arr, BucketIndex bid) {
    assert(arr->buckets[bid.bucket]->occupied[bid.index]);
    return &buckets[bid.bucket]->data[bid.index];
}

BucketIndex book(BucketArray* arr) {
    for (u32 i = 0; i < arr->bucket_count; i++) {
        Bucket* bucket = arr->buckets[i];
        if (bucket->alive_count == bucket->capacity)
            continue;
        for (u32 j = 0; j < bucket->capacity; j++) {
            if (!bucket->occupied[j]) {
                bucket->alive_count++;
                bucket->occupied[j] = true;
                return BucketIndex({i,j});
            }
        }
    }
}

template <typename T>
BucketIndex release(BucketArray* arr, BucketIndex idx) {
    assert(arr->buckets[bid.bucket]->occupied[bid.index]);
}

