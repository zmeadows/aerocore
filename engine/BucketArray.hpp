#pragma once

/*

#include <optional>
#include <vector>
#include <memory>

namespace __BucketArray__Internal__ {

template <typename T>
struct Bucket {
    // A memory buffer to store all the data in
    // It is not type T[] in order to allow for leaving all the items uninitialized until filled
    std::unique_ptr<char[]> buffer;

    //TODO: use bitset representation to save space here (not really a bit deal)
    std::unique_ptr<bool[]> occupied;

    const u32 capacity;

    // keep track of the count of objects in the bucket, so we can quickly skip full buckets
    // when looking for a place to insert a new object in the parent BucketArray<T>
    u32 count;

    Bucket(u32 size) : buffer(std::make_unique<char[]>(sizeof(T) * size))
                     , occupied(std::make_unique<bool[]>(sizeof(bool) * size))
                     , capacity(size)
                     , count(0)
    {
        for (u32 i = 0; i < capacity; i++) {
            occupied[i] = false;
        }
    }

    Bucket(const Bucket&) = delete;
    Bucket(Bucket&&) = delete;
    Bucket& operator=(const Bucket&) = delete;
    Bucket& operator=(Bucket&&) = delete;

    void release(u32 idx) {
        assert(occupied[idx]);
        data()[idx].~T();
        occupied[idx] = false;
        count--;
    }


    T* data(void) {
        return reinterpret_cast<T*>(buffer.get());
    }

    ~Bucket() {
        T* data = this->data();
        for (auto i = 0; i < capacity; i++) {
            if (occupied[i]) data[i].~T();
        }
    }
};

}

struct BucketIndex {
    u32 bucket;
    u32 slot;
};

template <typename T>
struct BucketArray {
    std::vector<std::unique_ptr<Bucket<T>>> buckets;
    const u32 bucket_size;

    BucketArray(const u32 _bucket_size)
        : bucket_size(_bucket_size)
        , buckets((Bucket**) malloc(sizeof(Bucket*)))
        , bucket_count(1)
    {
        buckets[0] = new Bucket(items_per_bucket, item_size_bytes);
    }

    Bucket(const Bucket&) = delete;
    Bucket(Bucket&&) = delete;
    Bucket& operator=(const Bucket&) = delete;
    Bucket& operator=(Bucket&&) = delete;

    T& const operator[](const BucketIndex bid);
    const T& const operator[](const BucketIndex bid) const;

    template <class... Args>
    BucketIndex insert(Args&&... args) {
        assert(count < capacity);

        for (u32 bucket_id = 0; bucket_id < buckets.size(); bucket_id++) {
            auto& bucket = buckets[i];

            if (bucket->count >= bucket->capacity)
                continue;

            for (u32 slot_id = 0; slot_id < bucket->capacity; slot_id++) {
                if (!bucket->occupied[slot_id]) {
                    new (&(bucket->data()[i])) T(args...);
                    bucket->count++;
                    return {bucket_id, slot_id};
                }
            }
        }

        // couldn't find an empty slot in the old buckets, so make a new bucket
        auto& new_bucket = buckets.emplace_back(std::make_unique<Bucket<T>>(bucket_size));
        new (&(new_bucket->data()[0])) T(args...);
        new_bucket->count++;
        return { buckets.size() - 1, 0 };
    }

    void release(BucketIndex idx) {
        buckets[idx.bucket]->release(idx.slot);
    }

};

*/
