#pragma once

#include "unstd/DynamicArray.hpp"
#include "unstd/unstdlib.hpp"
#include "unstd/types.hpp"
#include "Random.hpp"

// #define DEBUG_HASHTABLE(x) \
//    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "] :: " << x << std::endl)
#define DEBUG_HASHTABLE(x)

namespace {

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

inline u64 modulo(s64 i, s64 n) {
    return static_cast<u64>((i % n + n) % n);
}

}

template <typename T>
struct IdentityHasher {
    static u64 hash(T x) { return static_cast<u64>(x); }
};

template <typename K, typename V, typename Hasher>
struct DenseHashTable {
    buffer<u8> slots;
    u64 size;
    u64 capacity;
    f64 rehash_load_factor;
    u64 longest_probe;

    const K empty_sentinel;
    const K deleted_sentinel;

	DenseHashTable(void) = delete;
	DenseHashTable(u64 init_capacity, const K& empty, const K& deleted);
	DenseHashTable(const K& empty, const K& deleted) : DenseHashTable(64, empty, deleted) {}

    V& operator[](const K& key);
    const V& operator[](const K& key) const;
};

template <typename K, typename V, typename Hasher>
void deallocate(DenseHashTable<K,V,Hasher>& self) {
    self.slots.reset(nullptr);
    self.size = 0;
    self.capacity = 0;
    self.longest_probe = 0;
};


template <typename K, typename V, typename Hasher>
DenseHashTable<K,V,Hasher>::DenseHashTable(u64 init_capacity, const K& empty, const K& deleted)
    : slots(allocate_buffer<u8>(sizeof(KeyValuePair<K,V>) * init_capacity))
    , size(0)
    , capacity(init_capacity)
    , rehash_load_factor(0.5)
    , longest_probe(0)
    , empty_sentinel(empty)
    , deleted_sentinel(deleted)
{
    assert(this->empty_sentinel != this->deleted_sentinel);
    assert(this->capacity > 0);

    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(this->slots.get());
    for (u64 i = 0; i < this->capacity; i++) {
        cast_slots[i].key = this->empty_sentinel;
    }
}

template <typename K, typename V, typename Hasher>
void rehash(DenseHashTable<K,V,Hasher>& self, u64 new_capacity) {
    assert(new_capacity > self.capacity);

    DEBUG_HASHTABLE("Rehashing table from capacity of " << self.capacity << " to " << new_capacity);

    DenseHashTable<K,V,Hasher> new_table(new_capacity, self.empty_sentinel, self.deleted_sentinel);

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
    self.longest_probe = new_table.longest_probe;

    DEBUG_HASHTABLE("\t...success");
}

template <typename K, typename V, typename Hasher>
bool contains(DenseHashTable<K,V,Hasher>& self, const K& key) {
    return lookup(self, key) != nullptr;
}

template <typename K, typename V, typename Hasher>
const V* lookup(DenseHashTable<K,V,Hasher>& self, const K& lookup_key)
{
    assert(lookup_key != self.empty_sentinel && lookup_key != self.deleted_sentinel);

    DEBUG_HASHTABLE("Looking up key " << lookup_key << " in table..");

    const u64 N = self.capacity;
    u64 probe_index = Hasher::hash(lookup_key) % N;

    DEBUG_HASHTABLE("\t hash % N to bucket index: " << probe_index);

    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    u64 dib = 0;

    while (true) {
        KeyValuePair<K,V>& probed_pair = cast_slots[probe_index];

        if (probed_pair.key == lookup_key) {
            DEBUG_HASHTABLE("\t...found.");
            return &probed_pair.value;
        } else if (probed_pair.key == self.empty_sentinel) {
            DEBUG_HASHTABLE("\t...doesn't exist. Ran into empty slot.");
            return nullptr;
        }

        probe_index = (probe_index + 1) % N;
        dib++;

        if (dib > self.longest_probe) {
            DEBUG_HASHTABLE("\t...doesn't exist. Ran past maximum probe length.");
            return nullptr;
        }
    }
}

template <typename K, typename V, typename Hasher>
f64 load_factor(const DenseHashTable<K,V,Hasher>& self) {
    return static_cast<f64>(self.size) / static_cast<f64>(self.capacity);
}

template <typename K, typename V, typename Hasher>
void insert(DenseHashTable<K,V,Hasher>& self, K new_key, V new_value)
{
    assert(new_key != self.empty_sentinel && new_key != self.deleted_sentinel);

    DEBUG_HASHTABLE("Inserting into hash table: " << new_key << " --> " << new_value);

    if (load_factor(self) > self.rehash_load_factor) rehash(self, self.capacity * 2);

    const u64 N = self.capacity;
    s64 probe_index = Hasher::hash(new_key) % N;
    DEBUG_HASHTABLE("\t initial probe index = " << probe_index);

    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    u64 dib = 0;

    while (true) {
        DEBUG_HASHTABLE("distance from initial slot: " << dib);
        KeyValuePair<K,V>& probed_pair = cast_slots[probe_index];

        const u64 probed_bucket_start = Hasher::hash(probed_pair.key) % N;
        const u64 probed_dib = probe_index - probed_bucket_start;

        if (probed_pair.key == self.deleted_sentinel || probed_pair.key == self.empty_sentinel) {
            probed_pair.key = new_key;
            probed_pair.value = new_value;
            self.size++;
            DEBUG_HASHTABLE("\t...success!\t insert index = " << probe_index << "\t probe length = " << dib);
            if (dib > self.longest_probe) {
                self.longest_probe = dib;
                DEBUG_HASHTABLE("\t... and found new maximum probe length: " << self.longest_probe);
            }
            return;
        } else if (probed_pair.key == new_key) {
            probed_pair.value = new_value;
            DEBUG_HASHTABLE("\t...replaced old value for key.");
            return;
        } else if (probed_dib < dib) {
            DEBUG_HASHTABLE("\t...swapping robin hood style at index " << probe_index << " for keys " << new_key << " --> " << probed_pair.key);
			if (dib > self.longest_probe) {
                self.longest_probe = dib;
			}
            std::swap(probed_pair.key, new_key);
            std::swap(probed_pair.value, new_value);
            dib = probed_dib;
            DEBUG_HASHTABLE("\t...now moving element with dib: " << probed_dib);
        }

        probe_index = (probe_index + 1) % N;
        dib++;
    }
}

template <typename K, typename V, typename Hasher>
void remove(DenseHashTable<K,V,Hasher>& self, const K& key_to_delete)
{
    assert(key_to_delete != self.empty_sentinel && key_to_delete != self.deleted_sentinel);

    DEBUG_HASHTABLE("Removing key from hash table: " << key_to_delete);

    const u64 N = self.capacity;
    u64 probe_index = Hasher::hash(key_to_delete) % N;
    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    u64 dib = 0;

    while (true) {
        KeyValuePair<K,V>& probed_slot = cast_slots[probe_index];

        if (probed_slot.key == key_to_delete) {
            probed_slot.key = self.deleted_sentinel;
            self.size--;
            DEBUG_HASHTABLE("\t...success.");
            return;
        } else if (probed_slot.key == self.empty_sentinel) {
            DEBUG_HASHTABLE("\t...failed. Ran into empty key.");
            return;
        }

        probe_index = (probe_index + 1) % N;
        dib++;

        if (dib > self.longest_probe) {
            DEBUG_HASHTABLE("\t...doesn't exist. Ran past maximum probe length.");
            return;
        }
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
void for_each(const DenseHashTable<K,V,Hasher>& self, Callable&& f)
{
    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    for (auto i = 0; i < self.capacity; i++) {
        const K& key = cast_slots[i].key;
        if (key != self.empty_sentinel && key != self.deleted_sentinel) {
            f(key, cast_slots[i].value);
        }
    }
}

// rather than define complicated insane C++ 'iterators', just a simple higher-order function.
template <typename K, typename V, typename Hasher, typename Callable>
void for_each_with_delete(DenseHashTable<K,V,Hasher>& self, Callable&& f)
{
    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    for (auto i = 0; i < self.capacity; i++) {
        K& key = cast_slots[i].key;
        if (key != self.empty_sentinel && key != self.deleted_sentinel) {
            if (f(key, cast_slots[i].value)) {
                key = self.deleted_sentinel;
                self.size--;
            }
        }
    }
}

template <typename K, typename V, typename Hasher>
void debug_print(const DenseHashTable<K,V,Hasher>& self) {
    std::cout << "HASH TABLE @ " << &self << std::endl;
    std::cout << "SIZE: " << self.size << std::endl;
    std::cout << "CAPACITY: " << self.capacity << std::endl;
    std::cout << "LOAD_FACTOR: " << load_factor(self) << std::endl;
    std::cout << "LONGEST_PROBE: " << self.longest_probe << std::endl;

    KeyValuePair<K,V>* cast_slots = reinterpret_cast<KeyValuePair<K,V>*>(self.slots.get());

    for (auto i = 0; i < self.capacity; i++) {
        const K& key = cast_slots[i].key;
        if (key != self.empty_sentinel && key != self.deleted_sentinel) {
            std::cout << "[" << i << "]" << "\t" << key << " --> " << cast_slots[i].value << std::endl;
        }
    }

    std::cout << std::endl;
}
