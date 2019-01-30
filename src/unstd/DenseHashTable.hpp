#pragma once

#include "unstd/unstdlib.hpp"
#include "unstd/DynamicArray.hpp"

namespace {

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

template <typename T>
struct DefaultHash {
    static u32 hash(UUID::rep x) {
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
        auto x = reinterpret_cast<u32>(xptr);
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }
};

}

template <typename K, typename V, typename H = DefaultHash<K>>
struct DenseHashTable {
    u8* slots;
    u32 size;
    u32 capacity;
    f64 max_load_factor;
    u32 longest_probe;
    K empty_sentinel;
    K tombstone_sentinel;

	static DenseHashTable<K, V, H> create(u32 init_capacity, K empty, K tombstone);
	static DenseHashTable<K, V, H> create(K empty, K tombstone);
    static void destroy(DenseHashTable<K,V,H>* self);

    V& operator[](K key);
};

template <typename K, typename V, typename H>
DenseHashTable<K,V,H> DenseHashTable<K,V,H>::create(u32 init_capacity, K empty, K tombstone)
{
    assert(empty != tombstone && "DenseHashTable: empty and tombstone keys must be different!");
    assert(init_capacity > 0);

    DenseHashTable<K,V,H> table;

    table.slots            = memalloc<u8, KeyValuePair<K,V>>(init_capacity);
    table.size             = 0;
    table.capacity         = init_capacity;
    table.max_load_factor  = 0.7;
    table.longest_probe    = 0;
    table.empty_sentinel   = empty;
    table.tombstone_sentinel = tombstone;

    KeyValuePair<K,V>* cast_slots = recast_slots(&table);
    for (auto i = 0; i < table.capacity; i++) {
        cast_slots[i].key = table.empty_sentinel;
    }

    return table;
}

template <typename K, typename V, typename H>
DenseHashTable<K,V,H> DenseHashTable<K,V,H>::create(K empty, K tombstone) {
    return DenseHashTable<K,V,H>::create(16, empty, tombstone);
}

template <typename K, typename V, typename H>
KeyValuePair<K,V>* recast_slots(DenseHashTable<K,V,H>* self) {
    return reinterpret_cast<KeyValuePair<K,V>*>(self->slots);
}

template <typename K, typename V, typename H>
void DenseHashTable<K,V,H>::destroy(DenseHashTable<K,V,H>* self)
{
    free(self->slots);
    self->size = 0;
    self->capacity = 0;
    self->longest_probe = 0;
}


template <typename K, typename V, typename H>
bool contains(DenseHashTable<K,V,H>* self, K key) {
    return lookup(self, key) != nullptr;
}

template <typename K, typename V, typename H>
V* lookup(DenseHashTable<K,V,H>* self, K lookup_key)
{
    assert(lookup_key != self->empty_sentinel && lookup_key != self->tombstone_sentinel);

    u32 N = self->capacity;
    u32 probe_index = H::hash(lookup_key) % N;

    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    u32 dib = 0;

    while (true) {
        KeyValuePair<K,V>& probed_pair = cast_slots[probe_index];

        if (probed_pair.key == lookup_key) {
            return &probed_pair.value;
        } else if (probed_pair.key == self->empty_sentinel) {
            return nullptr;
        }

        probe_index = (probe_index + 1) % N;
        dib++;

        if (dib > self->longest_probe) return nullptr;
    }
}

template <typename K, typename V, typename H>
f64 load_factor(DenseHashTable<K,V,H>* self) {
    return static_cast<f64>(self->size) / static_cast<f64>(self->capacity);
}

template <typename K, typename V, typename H>
void insert(DenseHashTable<K,V,H>* self, K new_key, V new_value)
{
    assert(new_key != self->empty_sentinel && new_key != self->tombstone_sentinel);

    if (load_factor(self) > self->max_load_factor) rehash(self, self->capacity * 2);

    const u32 N = self->capacity;
    u32 probe_index = H::hash(new_key) % N;

    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    u32 dib = 0; // 'd'istance from 'i'nitial 'b'ucket

    while (true) {
        KeyValuePair<K,V>& probed_pair = cast_slots[probe_index];

        if (probed_pair.key == self->tombstone_sentinel || probed_pair.key == self->empty_sentinel) {
            probed_pair.key = new_key;
            probed_pair.value = new_value;
            self->size++;
            self->longest_probe = max(dib, self->longest_probe);
            return;
        } else if (probed_pair.key == new_key) {
            probed_pair.value = new_value;
            return;
        } else {
            u32 probed_dib = probe_index - H::hash(probed_pair.key) % N;
            if (probed_dib < dib) {
				self->longest_probe = max(dib, self->longest_probe);
                std::swap(probed_pair.key, new_key);
                std::swap(probed_pair.value, new_value);
                dib = probed_dib;
            }
        }

		probe_index = (probe_index + 1) % N;
        dib++;
    }
}

template <typename K, typename V, typename H>
void rehash(DenseHashTable<K,V,H>* self, u32 new_capacity)
{
    assert(new_capacity > self->capacity);

    auto new_table = DenseHashTable<K,V,H>::create(new_capacity, self->empty_sentinel,
                                                   self->tombstone_sentinel);

    KeyValuePair<K,V>* old_slots = recast_slots(self);

    for (auto i = 0; i < self->capacity; i++) {
        KeyValuePair<K,V>& slot = old_slots[i];
        if (slot.key != self->empty_sentinel && slot.key != self->tombstone_sentinel) {
            insert(&new_table, slot.key, slot.value);
        }
    }

    free(self->slots);
    *self = new_table;
}

template <typename K, typename V, typename H>
bool remove(DenseHashTable<K,V,H>* self, const K& key_to_delete)
{
    assert(key_to_delete != self->empty_sentinel && key_to_delete != self->tombstone_sentinel);

    const u32 N = self->capacity;
    u32 probe_index = H::hash(key_to_delete) % N;
    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    u32 dib = 0;

    while (true) {
        KeyValuePair<K,V>& probed_slot = cast_slots[probe_index];

        if (probed_slot.key == key_to_delete) {
            probed_slot.key = self->tombstone_sentinel;
            self->size--;
            return true;
        } else if (probed_slot.key == self->empty_sentinel) {
            return false;
        }

        probe_index = (probe_index + 1) % N;
        dib++;

        if (dib > self->longest_probe) {
            return false;
        }
    }
}

template <typename K, typename V, typename H>
V& DenseHashTable<K,V,H>::operator[](K key)
{
    V* result = lookup(this, key);
    assert(result != nullptr);
    return *result;
}

template <typename K, typename V, typename H, typename Callable>
void for_each(const DenseHashTable<K,V,H>* self, Callable&& f)
{
    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    for (auto i = 0; i < self->capacity; i++) {
        const K& key = cast_slots[i].key;
        if (key != self->empty_sentinel && key != self->tombstone_sentinel) {
            f(key, cast_slots[i].value);
        }
    }
}

template <typename K, typename V, typename H, typename Callable>
void for_each_with_delete(DenseHashTable<K,V,H>* self, Callable&& f)
{
    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    for (auto i = 0; i < self->capacity; i++) {
        K& key = cast_slots[i].key;
        if (key != self->empty_sentinel && key != self->tombstone_sentinel) {
            if (f(key, cast_slots[i].value)) {
                key = self->tombstone_sentinel;
                self->size--;
            }
        }
    }
}

template <typename K, typename V, typename H>
void debug_print(DenseHashTable<K,V,H>* self)
{
    std::cout << "HASH TABLE @ " << &self << std::endl;
    std::cout << "SIZE: " << self->size << std::endl;
    std::cout << "CAPACITY: " << self->capacity << std::endl;
    std::cout << "LOAD_FACTOR: " << load_factor(self) << std::endl;
    std::cout << "LONGEST_PROBE: " << self->longest_probe << std::endl;

    KeyValuePair<K,V>* cast_slots = recast_slots(self);

    for (auto i = 0; i < self->capacity; i++) {
        K& key = cast_slots[i].key;
        if (key != self->empty_sentinel && key != self->tombstone_sentinel) {
            std::cout << "[" << i << "]" << "\t" << key << " --> " << cast_slots[i].value << std::endl;
        }
    }

    std::cout << std::endl;
}

