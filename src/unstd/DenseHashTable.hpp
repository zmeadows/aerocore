#pragma once

#include "unstd/unstdlib.hpp"
#include "unstd/DynamicArray.hpp"

#include <algorithm>

namespace {

template <typename T>
inline bool is_power_of_two(T n) {
    return (n & (n - 1)) == 0;
}

template <typename T>
struct DefaultHash {
    static u32 hash(T x) {
        static_assert(sizeof(T) == 0, "DefaultHash not implemented for this type!");
        return 0;
    }
};

template <>
struct DefaultHash<u32> {
    static u32 hash(u32 x) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }
};

template <typename T>
struct DefaultHash<T*> {
    static u32 hash(T* xptr) {
        return DefaultHash<u32>::hash((u32)(size_t)(xptr));
    }
};

}

template <typename K, typename V, typename H = DefaultHash<K>>
class DenseHashTable {
    using Self = DenseHashTable<K,V,H>;

    struct KV {
        K key;
        V value;
    };

    KV* slots;
    u64 count;
    u64 capacity;
    u64 longest_probe;
    f64 max_load_factor;
    K empty_sentinel;
    K tombstone_sentinel;

public:

    static Self create(u64 init_capacity, K empty, K tombstone) {
        assert(empty != tombstone &&
               "DenseHashTable: empty and tombstone keys must be different!");

        assert(is_power_of_two(init_capacity) &&
               "DenseHashTable: Capacity must always be a power of two!");

        Self table;

        table.slots           = memalloc<KV>(init_capacity);
        table.count            = 0;
        table.capacity        = init_capacity;
        table.max_load_factor = 0.7;
        table.longest_probe   = 0;

        table.empty_sentinel     = empty;
        table.tombstone_sentinel = tombstone;

        for (auto i = 0; i < table.capacity; i++) {
            table.slots[i].key = table.empty_sentinel;
        }

        return table;
    }

    static Self create(K empty, K tombstone) {
        return Self::create(2, empty, tombstone);
    }

    void destroy() {
        Self& self = *this;
        free(self.slots);
        self.slots = nullptr;
        self.count = 0;
        self.capacity = 0;
        self.longest_probe = 0;
    }

    inline u64 size(void) const { return this->count; }

    V* lookup(K lookup_key) {
        Self& self = *this;

        assert(lookup_key != self.empty_sentinel
               && "DenseHashTable: Attempted to lookup the empty sentinel key in the table!");

        assert(lookup_key != self.tombstone_sentinel
               && "DenseHashTable: Attempted to lookup the tombostone sentinel key in the table!");

        const u64 N = self.capacity - 1;
        u32 probe_index = H::hash(lookup_key) & N;

        u64 dib = 0;

        while (true) {
            KV& probed_pair = self.slots[probe_index];

            if (probed_pair.key == lookup_key) {
                return &probed_pair.value;
            } else if (probed_pair.key == self.empty_sentinel) {
                return nullptr;
            }

            probe_index = (probe_index + 1) & N;
            dib++;

            if (dib > self.longest_probe) return nullptr;
        }
    }

    const V* lookup(K lookup_key) const {
        return static_cast<const V*>(this->lookup(lookup_key));
    }

    // TODO: emplace version
    V* insert(K new_key, V new_value) {
        Self& self = *this;

        assert(new_key != self.empty_sentinel
               && "DenseHashTable: Attempted to insert the empty sentinel key in the table!");

        assert(new_key != self.tombstone_sentinel
               && "DenseHashTable: Attempted to insert the tombostone sentinel key in the table!");

        if (self.load_factor() > self.max_load_factor) self.rehash(self.capacity * 2);

        const u64 N = self.capacity - 1;
        u32 probe_index = H::hash(new_key) & N;

        u64 dib = 0; // 'd'istance from 'i'nitial 'b'ucket

        while (true) {
            KV& probed_pair = self.slots[probe_index];

            if (probed_pair.key == self.tombstone_sentinel || probed_pair.key == self.empty_sentinel) {
                probed_pair.key = new_key;
                probed_pair.value = new_value;
                self.count++;
                self.longest_probe = max(dib, self.longest_probe);
                return &probed_pair.value;
            } else if (probed_pair.key == new_key) {
                probed_pair.value = new_value;
                return &probed_pair.value;
            } else {
                u64 probed_dib = probe_index - H::hash(probed_pair.key) & N;
                if (probed_dib < dib) {
                    self.longest_probe = max(dib, self.longest_probe);
                    std::swap(probed_pair.key, new_key);
                    std::swap(probed_pair.value, new_value);
                    dib = probed_dib;
                }
            }

            probe_index = (probe_index + 1) & N;
            dib++;
        }
    }

    inline bool contains(K key) {
        return this->lookup(key) != nullptr;
    }

    inline f64 load_factor() const {
        return static_cast<f64>(this->count) / static_cast<f64>(this->capacity);
    }


    bool remove(const K& key_to_delete) {
        Self& self = *this;

        assert(key_to_delete != self.empty_sentinel &&
               "DenseHashMap: Attempted to remove empty sentinel key from map!");

        assert(key_to_delete != self.tombstone_sentinel &&
               "DenseHashMap: Attempted to remove tombstone sentinel key from map!");

        const u64 N = self.capacity - 1;
        u64 probe_index = H::hash(key_to_delete) & N;

        u64 dib = 0;

        while (true) {
            KV& probed_slot = self.slots[probe_index];

            if (probed_slot.key == key_to_delete) {
                probed_slot.key = self.tombstone_sentinel;
                self.count--;
                return true;
            } else if (probed_slot.key == self.empty_sentinel) {
                return false;
            }

            probe_index = (probe_index + 1) & N;
            dib++;

            if (dib > self.longest_probe) {
                return false;
            }
        }
    }

    void rehash(u64 new_capacity) {
        Self& self = *this;

        assert(new_capacity > self.capacity);

        assert(is_power_of_two(new_capacity) &&
               "DenseHashTable: Table capacity must be a power of two!");

        Self new_table = Self::create(new_capacity, self.empty_sentinel,
                                      self.tombstone_sentinel);

        for (size_t i = 0; i < self.capacity; i++) {
            KV& slot = self.slots[i];
            if (slot.key != self.empty_sentinel && slot.key != self.tombstone_sentinel) {
                new_table.insert(slot.key, slot.value);
            }
        }

        free(self.slots);
        self = new_table;
    }

    V& operator[](K key)
    {
        Self& self = *this;
        V* result = self.lookup(key);
        assert(result != nullptr && "DenseHashMap: Called operator[] with non-existent key!");
        return *result;
    }
};

