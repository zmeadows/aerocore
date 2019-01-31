#pragma once

#include "unstd/unstdlib.hpp"

struct DynamicBitSet {
    u64* chunks = nullptr;
    u64 chunk_count = 0;
};

void set_bit(DynamicBitSet* self, u64 bit);
bool check_bit(DynamicBitSet* self, u64 bit);
void unset_bit(DynamicBitSet* self, u64 bit);
bool is_subset_of(DynamicBitSet* subset_bits, DynamicBitSet* test_bits);
void destroy(DynamicBitSet* self);
void debug_print(DynamicBitSet* self);
