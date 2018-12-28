#include "BucketArray.hpp"

/*

namespace __BucketArray__Internal__ {

void
add_empty_bucket(BucketArray* arr)
{
    Bucket** new_buckets = realloc(arr->buckets, (arr->bucket_count + 1) * sizeof(Bucket*));
    assert(new_buckets && "failed to reallocate memory for bucket array.");
    buckets = new_buckets;
    arr->bucket_count += 1;
    buckets[bucket_count-1] = new Bucket(arr->items_per_bucket, arr->item_size_bytes);
}

void
destroy_bucket(Bucket* bucket) {
    free(bucket->occupied);
    free(bucket->data);
}

}


void
destroy_bucket_array(BucketArray* arr)
{
    for (u32 i = 0; i < arr->bucket_count; i++) {
        destroy_bucket(buckets[i]);
    }

    free(buckets);
}


BucketIndex reserve_empty_slot(BucketArray* arr) {
    // 1. Find a free slot in one of the buckets

    for (u32 i = 0; i < arr->bucket_count; i++) {
        Bucket* bucket = arr->buckets[i];

        // obviously can't add an item to a full bucket
        if (bucket->occupied_count == bucket->capacity)
            continue;

        // sanity check
        assert(bucket->occupied_count < bucket->capacity);

        for (u32 j = 0; j < bucket->capacity; j++) {
            if (!bucket->occupied[j]) {
                bucket->occupied_count++;
                bucket->occupied[j] = true;
                return BucketIndex({i,j});
            }
        }
    }

    // 2. If no slot is found in (1), create a new bucket
    // and occupy the first slot
    add_empty_bucket(arr);
    buckets[arr->bucket_count-1]->occupied[0] = true;
    return BucketIndex({arr->bucket_count-1, 0});
}

void release_slot(BucketArray* arr, BucketIndex bid) {
    assert(arr->buckets[bid.bucket]->occupied[bid.slot]);
    arr->buckets[bid.bucket]->occupied[bid.slot]) = false;
}

u8* const BucketArray::operator[](const BucketIndex bid)
{
    assert(bid.bucket < this->bucket_count);
    assert(bid.slot < this->items_per_bucket);
    return this->buckets[bid.bucket]->data + bid.slot * this->item_size_bytes;
}

const u8* const BucketArray::operator[](const BucketIndex bid) const
{
    assert(bid.bucket < this->bucket_count);
    assert(bid.slot < this->items_per_bucket);
    return this->buckets[bid.bucket]->data + bid.slot * this->item_size_bytes;
}

*/
