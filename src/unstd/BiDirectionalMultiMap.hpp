#pragma once

#include "unstd/DenseHashTable.hpp"
#include "unstd/ArraySet.hpp"

template <typename T1, typename T2>
struct BiDirectionalMultiMap {
    DenseHashTable<T1, ArraySet<T2>> forward_map;
    DenseHashTable<T2, ArraySet<T1>> backward_map;
};

template <typename T1, typename T2>
insert(BiDirectionalMultiMap<T1,T2>* self, T1 x, T2 y) {
    ArraySet<T2>* forward_set = lookup(self->forward_map, x);

    if (!forward_set) {
        ArraySet<T2> new_set;
        insert(&new_set, y);
        insert(self, x, new_set);
    } else {
        insert(forward_set, y);
    }

    ArraySet<T1>* backward_set = lookup(self->backward_map, y);
    if (!backward_set) {
        ArraySet<T1> new_set;
        insert(&new_set, x);
        insert(self, y, new_set);
    } else {
        insert(backward_set, x);
    }
}

template <typename T1, typename T2>
ArraySet<T2>* forward_matches(BiDirectionalMultiMap<T1,T2>* self, T1 x) {
    return lookup(self->forward_map, x);
}

template <typename T1, typename T2>
ArraySet<T1>* backward_matches(BiDirectionalMultiMap<T1,T2>* self, T2 x) {
    return lookup(self->backward_map, y);
}
