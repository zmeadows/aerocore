#include "BucketArray.hpp"

void release_slot(BucketBase* self, u32 slot_idx) {
    assert(self->filled_slots > 0 && self->occupied[slot_idx]);
    self->occupied[slot_idx] = false;
    self->filled_slots--;
}

u16 find_open_slot(BucketBase* self) {
    for (s32 i = 0; i < self->capacity; i++) {
        if (!self->occupied[i])
            return i;
    }
    return self->capacity;
}

bool is_full(BucketBase* self) {
    return self->filled_slots == self->capacity;
}

u64 size(const BucketArrayBase* self) {
    u64 sum = 0;
    for (BucketBase* bucket : self->buckets) {
        sum += (u64) bucket->filled_slots;
    }
    return sum;
}

void remove_item(BucketArrayBase* self, const BucketIndex index) {
    release_slot(self->buckets[index.bucket], index.slot);
}

std::ostream& operator<<(std::ostream& os, const BucketIndex index) {
    os << "(" << index.bucket << "," << index.slot;
    return os;
}
