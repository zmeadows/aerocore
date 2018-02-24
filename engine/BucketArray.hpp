#pragma once

template <typename T>
struct BucketIndex {
    const u32 id;
    const u32 index;

    // on destructor call, grab entity manager and release
};

template <typename T, unsigned N>
struct Bucket {
    u32* m_data;
};

template <typename T, unsigned N>
struct BucketArray {
    u32* m_data;
    u32 total_capacity;
    const u32 items_per_bucket;
};

