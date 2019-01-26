#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/unstdlib.hpp"
#include "unstd/types.hpp"

namespace {

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

}

template <typename K, typename V, typename Hasher>
struct DenseHashTable {
    buffer<u8> slots = nullptr;
    u64 size = 0; // number of filled slots
    u64 capacity = 0; // number of slots
    f64 rehash_load_factor = 0.7;

    const K empty_sentinel;
    const K deleted_sentinel;

	DenseHashTable(void) = delete;
	DenseHashTable(u64 init_capacity, const K& empty, const K& deleted);
	DenseHashTable(const K& empty, const K& deleted) : DenseHashTable(64, empty, deleted) {}

    V& operator[](const K& key);
    const V& operator[](const K& key) const;
};

template <typename K, typename V, typename Hasher>
DenseHashTable<K,V,Hasher>::DenseHashTable(u64 init_capacity, const K& empty, const K& deleted)
    : count(0), empty_sentinel(empty), deleted_sentinel(deleted)
{
    assert(self.empty_sentinel != self.deleted_sentinel);
    reserve(this->slots, init_capacity);
    this->slots.size = init_capacity;

    for (u64 i = 0; i < this->size; i++) {
        this->slots[i].key = this->empty_sentinel;
    }
}

template <typename K, typename V, typename Hasher>
void rehash(DenseHashTable<K,V,Hasher>& self, u64 new_capacity) {
    assert(new_capacity > self.capacity);

    DenseHashTable<K,V,Hasher> new_table(new_capacity, self.empty, self.deleted);

    KeyValuePair<K,V>* old_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    for (auto i = 0; i < self.capacity; i++) {
        const KeyValuePair<K,V>& slot = old_slots[i];
        if (slot.key != self.empty_sentinel && slot.key != self.deleted_sentinel) {
            insert(new_table, slot.key, slot.value);
        }
    }

    self.slots.swap(new_table.slots);
    self.size = new_table.size;
    self.capacity = new_capacity;
}


template <typename K, typename V, typename Hasher>
const V* lookup(DenseHashTable<K,V,Hasher>& self, const K& lookup_key)
{
    assert(lookup_key != self.empty_sentinel && lookup_key != self.deleted_sentinel);

    const u64 N = self.size;
    u64 probe_index = Hasher::hash(lookup_key) % N;

    while (true) {
        KeyValuePair<K,V>& probed_pair = self.slots[probe_index];

        if (probed_pair.key == lookup_key) {
            return &probed_pair.value;
        } else if (probed_pair.key == self.empty_sentinel) {
            return nullptr;
        }
        probe_index = (probe_index + 1) % N;
    }
}

template <typename K, typename V, typename Hasher>
f64 load_factor(const DenseHashTable<K,V,Hasher>& self) {
    return static_cast<f64>(self.size) / static_cast<f64>(self.capacity);
}

template <typename K, typename V, typename Hasher>
void insert(DenseHashTable<K,V,Hasher>& self, const K& new_key, const V& new_value)
{
    assert(new_key != self.empty_sentinel && new_key != self.deleted_sentinel);

    if (load_factor(self) > self.rehash_load_factor) rehash(self);

    const u64 N = self.slots.size;
    u64 probe_index = Hasher::hash(new_key) % N;

    while (true) {
        KeyValuePair<K,V>& probed_pair = self.slots[probe_index];

        if (probed_pair.key == self.deleted_sentinel || probed_pair.key == self.empty_sentinel) {
            probed_pair.key = new_key;
            probed_pair.value = new_value;
            self.count++;
            return;
        } else if (probed_pair.key == new_key) {
            probed_pair.value = new_value;
            return;
        }

        probe_index = (probe_index + 1) % N;
    }
}

template <typename K, typename V, typename Hasher>
void remove(DenseHashTable<K,V,Hasher>& self, const K& key_to_delete)
{
    assert(key_to_delete != self.empty_sentinel && key_to_delete != self.deleted_sentinel);

    const u64 N = self.slots.size;
    u64 probe_index = Hasher::hash(key_to_delete) % N;

    while (true) {
        KeyValuePair<K,V>& probed_slot = self.slots[probe_index];

        if (probed_slot.key == key_to_delete) {
            probed_slot.key = self.deleted_sentinel;
            self.count--;
        } else if (probed_slot.key == self.empty_sentinel) {
            return;
        }

        probe_index = (probe_index + 1) % N;
    }
}

template <typename K, typename V, typename Hasher>
V& DenseHashTable<K,V,Hasher>::operator[](const K& key) {
    const V* result = lookup(*this, key);
    assert(result != nullptr);
    return *result;
}

template <typename K, typename V, typename Hasher>
const V& DenseHashTable<K,V,Hasher>::operator[](const K& key) const {
    const V* const result = lookup(*this, key);
    assert(result != nullptr);
    return *result;
}

// rather than define complicated insane C++ 'iterators', just a simple higher-order function.
template <typename K, typename V, typename Hasher, typename Callable>
void for_each(DenseHashTable<K,V,Hasher>& self, Callable&& f)
{
    for (auto i = 0; i < self.slots.size; i++) {
        const K& key = self.slots[i].key;
        if (key != self.empty_sentinel && key != self.deleted_sentinel) {
            f(key, self.slots[i].value);
        }
    }
}

