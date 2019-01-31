#include "unstd/DynamicBitSet.hpp"

#include <iostream>

namespace {

const u64 ONE = 1;

void reallocate(DynamicBitSet* self, u64 new_chunk_count) {
    if (self->chunk_count >= new_chunk_count) return;

    u64* new_chunks = memallocz<u64>(new_chunk_count);

    for (auto chunk_index = 0; chunk_index < self->chunk_count; chunk_index++) {
        new_chunks[chunk_index] = self->chunks[chunk_index];
    }

    free(self->chunks);
    self->chunks = new_chunks;
    self->chunk_count = new_chunk_count;
}

}

void debug_print(DynamicBitSet* self) {
    for (auto i = 0; i < self->chunk_count; i++) {
        u64 chunk = self->chunks[i];
        for (auto j = 0; j < 64; j++) {
            if ((chunk >> j) & ONE) {
                std::cout << "1";
            } else {
                std::cout << "0";
            }
        }
        std::cout << " ";
    }

    std::cout << std::endl;
}

bool check_bit(DynamicBitSet* self, u64 bit) {
    u64 chunk_index = bit / 64;
    u64 required_chunks = chunk_index + 1;

    if (self->chunk_count < required_chunks) {
        return false;
    }

    u64 bit_index = bit % 64;

    return static_cast<bool>((self->chunks[chunk_index] >> bit_index) & ONE);
}

void set_bit(DynamicBitSet* self, u64 bit) {
    u64 chunk_index = bit / 64;

    u64 required_chunks = chunk_index + 1;

    if (self->chunk_count < required_chunks) {
        reallocate(self, required_chunks);
    }

    u64 bit_index = bit % 64;

    // std::cout << "setting chunk index: " << chunk_index << std::endl;
    // std::cout << "setting bit index: " << bit_index << std::endl;

    self->chunks[chunk_index] |= (ONE << bit_index);
}

void unset_bit(DynamicBitSet* self, u64 bit) {
    u64 chunk_index = bit / 64;

    u64 required_chunks = chunk_index + 1;

    if (self->chunk_count < required_chunks) {
        return;
    }

    u64 bit_index = bit % 64;

    self->chunks[chunk_index] &= ~(ONE << bit_index);
}

bool is_subset_of(DynamicBitSet* subset_bits, DynamicBitSet* test_bits) {
    DynamicBitSet* smallest_bitset = subset_bits->chunk_count < test_bits->chunk_count ? subset_bits : test_bits;

    for (auto i = 0; i < smallest_bitset->chunk_count; i++) {
        if ((subset_bits->chunks[i] & test_bits->chunks[i]) != subset_bits->chunks[i])
            return false;
    }

    if (subset_bits->chunk_count == test_bits->chunk_count || smallest_bitset == subset_bits) {
        return true;
    }

    for (auto i = 0; i < subset_bits->chunk_count; i++) {
        if (subset_bits->chunks[i] != 0)
            return false;
    }

    return true;
}

void destroy(DynamicBitSet* self) {
    free(self->chunks);
    self->chunks = nullptr;
    self->chunk_count = 0;
}
