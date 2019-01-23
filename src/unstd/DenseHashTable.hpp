#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/unstdlib.hpp"
#include "unstd/types.hpp"

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

template <typename K, typename V, typename Hasher>
struct DenseHashTable : NonCopyable {
	//TODO: don't use DynamicArray?
    DynamicArray<KeyValuePair<K,V>> slots;

    const K empty_sentinel;
    const K deleted_sentinel;

    DenseHashTable<K,V,Hasher>() = delete;
    DenseHashTable<K,V,Hasher>(u64 init_capacity, const K& empty, const K& deleted);
    DenseHashTable<K,V,Hasher>(const K& empty, const K& deleted);
    ~DenseHashTable<K,V,Hasher>(void);

    V& operator[](const K& key);
    const V& operator[](const K& key) const;

};

template <typename K, typename V, typename Hasher>
DenseHashTable<K, V, Hasher>::~DenseHashTable(void) {
    for (u64 i = 0; i < size(this->slots); i++) {
        const K& key = this->slots[i].key;
        if (key != this->empty_sentinel && key != this->deleted_sentinel)
            this->slots[i].value.~V();
    }

	//TODO: don't use DynamicArray
	slots.size = 0;
    clear(slots);
}

template <typename K, typename V, typename Hasher>
DenseHashTable<K,V,Hasher>::DenseHashTable(u64 init_capacity, const K& empty, const K& deleted)
    : empty_sentinel(empty), deleted_sentinel(deleted)
{
    reserve(this->slots, init_capacity);
    this->slots.size = init_capacity;

    for (u64 i = 0; i < size(this->slots); i++) {
        this->slots[i].key = this->empty_sentinel;
    }
}

template <typename K, typename V, typename Hasher>
DenseHashTable<K,V,Hasher>::DenseHashTable(const K& empty, const K& deleted)
    : DenseHashTable<K,V,Hasher>(64, empty, deleted)
{ }

template <typename K, typename V, typename Hasher>
const V* lookup(DenseHashTable<K,V,Hasher>& self, const K& lookup_key)
{
    assert(lookup_key != self.empty_sentinel && lookup_key != self.deleted_sentinel);

    const u64 N = size(self.slots);
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
void insert(DenseHashTable<K,V,Hasher>& self, const K& new_key, const V& new_value)
{
    assert(new_key != self.empty_sentinel && new_key != self.deleted_sentinel);
    //TODO: check if extending table is needed
		// if so, allocate new larger buffer and re-hash

    const u64 N = self.slots.size;
    u64 probe_index = Hasher::hash(new_key) % N;

    while (true) {
        KeyValuePair<K,V>& probed_pair = self.slots[probe_index];

        if (probed_pair.key == self.deleted_sentinel || probed_pair.key == self.empty_sentinel) {
            probed_pair.key = new_key;
            probed_pair.value = new_value;
            return;
        } else if (probed_pair.key == new_key) {
            probed_pair.value = new_value;
            return;
        }

        probe_index = (probe_index + 1) % N;
    }
}

template <typename K, typename V, typename Hasher>
void remove(DenseHashTable<K,V,Hasher>& self, const K& key)
{
    assert(lookup_key != self.sentinel && lookup_key != self.deleted_sentinel);
    double_hash_probe probe = build_probe(self, new_key);

    while (true) {
        const u64 probe_index = probe.next();

        auto& probed_KeyValuePair = self.slots[probe_index];

        if (probed_KeyValuePair.key == key) {
            probed_KeyValuePair.key = self.deleted_sentinel;
        } else if (probed_KeyValuePair.key == self.empty_sentinel) {
            return;
        }
    }
}

template <typename K, typename V, typename Hasher>
V& DenseHashTable<K,V,Hasher>::operator[](const K& key) { return lookup(*this, key); }

template <typename K, typename V, typename Hasher>
const V& DenseHashTable<K,V,Hasher>::operator[](const K& key) const { return lookup(*this, key); }

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

// template <typename T>
// u32 partition(Growableslots<T>& self, u32 low, u32 high)
// {
//     const T& pivot = self[high];
//
//     for (u32 i = low - 1, u32 j = low; j <= high - 1; j++)
//     {
//         if (arr[j] <= pivot) {
//             i++;
//             swap(self[i], self[j]);
//         }
//     }
//     swap(self[i + 1], self[high]);
//
//     return (i + 1);
// }
//
// template <typename T>
// void quick_sort(Growableslots<T>& self, u32 low, u32 high)
// {
//     if (low < high) {
//         u32 pi = partition(arr, low, high);
//         quick_sort(self, low, pi - 1);
//         quick_sort(self, pi + 1, high);
//     }
// }
// template <typename T>
// void quick_sort(Growableslots<T>& self)
// {
//     quick_sort(self, 0, self._size);
// }
