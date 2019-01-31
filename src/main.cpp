#include "Game.hpp"
#include <SDL.h>
#include <assert.h>
#include <cstdio>
#include <iostream>

#include "unstd/DenseHashTable.hpp"
#include "unstd/DynamicBitSet.hpp"
#include "BucketArray.hpp"
#include "Random.hpp"

/*
void test_table(int entries, int times) {
    for (auto test_num = 1; test_num <= times; test_num++) {
        DenseHashTable<u32, u32, IdentityHasher<u32>> hash_table(1, 0, 1);
        std::vector<u32> keys, values;

        for (auto i = 0; i < entries; i++) {
            u32 x = xorshf96();
            if (x != 0 && x != 1 && lookup(hash_table, x) == nullptr) {
                u32 y = xorshf96();
                insert(hash_table, x, y);
                keys.push_back(x);
                values.push_back(y);
            }
        }

        std::cout << "CREATED INITIAL TABLE" << std::endl;
        std::cout << "KEYS VECTOR SIZE: " << keys.size() << std::endl;
        std::cout << "VALUES VECTOR SIZE: " << values.size() << std::endl;
        debug_print(hash_table);

        for (int i = 0; i < keys.size(); i++) {
            const u32* result = lookup(hash_table, keys[i]);
            int value = values[i];
            std::cout << "CHECKING FOR KEY: " << keys[i] << std::endl;
			assert(result != nullptr);
            assert(*result == value);
        }

        auto kit = keys.begin();
        auto vit = values.begin();

        std::cout << "REMOVING RANDOM TABLE ENTRIES" << std::endl;

        while (kit != keys.end() && vit != values.end()) {
            if (coin_flip()) {
                remove(hash_table, *kit);
                kit = keys.erase(kit);
                vit = values.erase(vit);
            } else {
                kit++;
                vit++;
            }
        }

        std::cout << "REMOVED APPROX HALF RANDOM TABLE ENTRIES" << std::endl;
        std::cout << "KEYS VECTOR SIZE: " << keys.size() << std::endl;
        std::cout << "VALUES VECTOR SIZE: " << values.size() << std::endl;
        debug_print(hash_table);

        for (int i = 0; i < keys.size(); i++) {
            int result = *lookup(hash_table, keys[i]);
            int value = values[i];
            assert(result == value);
        }

        std::cout << "FINISHED TEST # " << test_num << std::endl;
    }
}
*/

// void test_bucket_array(void) {
//     BucketArray<int>* arr = BucketArray<int>::allocate(1000);
//
//     auto index = insert(arr, 3);
//     std::cout << index.bucket << " , " << index.slot << " :: " << arr[index] << std::endl;
//     arr[index]++;
//     std::cout << index.bucket << " , " << index.slot << " :: " << arr[index] << std::endl;
//
//     auto new_index = insert(arr, 10);
//     std::cout << new_index.bucket << " , " << new_index.slot << " :: " << arr[new_index] << std::endl;
//     arr[new_index]++;
//     std::cout << new_index.bucket << " , " << new_index.slot << " :: " << arr[new_index] << std::endl;
//     std::cout << index.bucket << " , " << index.slot << " :: " << arr[index] << std::endl;
//
//     BucketArray<int>::destroy(arr);
// }


void test_ArraySet(void) {
    ArraySet<int> test;
    reserve_memory(test, 1);
    debug_print(test);

    insert(test, 1);
    debug_print(test);
    remove(test, 1);
    debug_print(test);
    insert(test, 1);
    debug_print(test);
    insert(test, 2);
    debug_print(test);
    insert(test, 3);
    debug_print(test);
    insert(test, 4);
    debug_print(test);
    insert(test, -1);
    debug_print(test);
    insert(test, 10);
    debug_print(test);
    insert(test, 6);
    debug_print(test);
    remove(test, 1653456765);
    debug_print(test);
    remove(test, 2);
    debug_print(test);
    insert(test, 2);
    debug_print(test);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(9999));

    test_ArraySet();

    DynamicBitSet setA;
    DynamicBitSet setB;

    set_bit(&setA, 3);
    debug_print(&setA);
    assert(check_bit(&setA, 3));
    set_bit(&setA, 4);
    debug_print(&setA);
    assert(check_bit(&setA, 4));
    set_bit(&setA, 5);
    debug_print(&setA);
    assert(check_bit(&setA, 5));
    unset_bit(&setA, 5);
    debug_print(&setA);
    assert(!check_bit(&setA, 5));
    set_bit(&setA, 5);
    debug_print(&setA);
    assert(check_bit(&setA, 5));
    set_bit(&setA, 111);
    debug_print(&setA);
    assert(check_bit(&setA, 111));

    set_bit(&setB, 3);
    set_bit(&setB, 4);
    set_bit(&setB, 5);
    set_bit(&setB, 111);
    set_bit(&setB, 99);
    set_bit(&setB, 30);

	debug_print(&setA);
	debug_print(&setB);

    assert(is_subset_of(&setA, &setB));

    destroy(&setA);
    destroy(&setB);

    Game game = Game();
    game.go();

    return 0;
}
