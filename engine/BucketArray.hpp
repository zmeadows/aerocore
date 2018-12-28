#pragma once

// Simple c-style bucket array. Does not manage lifetime of contained data.
// The only purpose of this data structure is to hold arbitrary data in continguous memory chunks.

/*
namespace __BucketArray__Internal__ {

template <typename T>
struct Bucket {
    const u32 capacity;
    bool* occupied;
    T* data;
    u32 occupied_count;

    Bucket(u32 bucket_size, u32 item_size)
        : capacity(bucket_size)
          , item_size_bytes(item_size_bytes)
          , occupied_count(0)
          , occupied((bool*) malloc(sizeof(bool) * capacity))
          , data((u8*) malloc(item_size_bytes * capacity))
          , occupied_count(0)
    {
        assert(occupied);
        assert(data);
        for (u32 i = 0; i < capacity; i++) { occupied[i] = false; }
    }
};

void add_empty_bucket(BucketArray* arr);
void destroy_bucket(Bucket* bucket);

}

struct BucketIndex {
    const u32 bucket;
    const u32 slot;
};

struct BucketArray {
    const u32 items_per_bucket;
    const u32 item_size_bytes;
    Bucket** buckets;
    u32 bucket_count;

    u8* const operator[](const BucketIndex bid);
    const u8* const operator[](const BucketIndex bid) const;

    BucketArray(const u32 bucket_size, const u32 item_size)
        : items_per_bucket(bucket_size)
        , item_size_bytes(item_size)
        , buckets((Bucket**) malloc(sizeof(Bucket*)))
        , bucket_count(1)
    {
        buckets[0] = new Bucket(items_per_bucket, item_size_bytes);
    }
};

void destroy_bucket_array(BucketArray* arr);
BucketIndex reserve_empty_slot(BucketArray* arr);
void* release_slot_and_get_data(BucketArray* arr, BucketIndex bid);
*/
