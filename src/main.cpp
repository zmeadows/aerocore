#include <SDL.h>
#include <time.h>
#include "Game.hpp"
#include <iostream>
#include <assert.h>

#include "unstd/DynamicArray.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/DenseHashTable.hpp"

#include "UUID.hpp"


struct UUIDHasher {
    static u64 hash(const UUID& uuid) { return uuid.unwrap(); }
};

struct IntHasher {
    static u64 hash(int x) { return static_cast<u64>(x); }
};

int main(int argc, char** argv) {
    srand (static_cast<unsigned>(time(nullptr)));

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

    DenseHashTable<int,int,IntHasher> hash_table(1e6, 0, 1);

    std::vector<int> keys, values;

    for (int i = 0; i < 5e5; i++) {
        int x = rand();
        if (x != 0 && x != 1 && lookup(hash_table, x) == nullptr) {
            int y = rand();
            insert(hash_table, x, y);

            keys.push_back(x);
            values.push_back(y);
        }
    }

    std::cout << "TABLE SIZE: " << hash_table.size << std::endl;

    for (int i = 0; i < keys.size(); i++) {
        int result = *lookup(hash_table, keys[i]);
        int value = values[i];
        if (result != value) 
            std::cout << "result: " << result << " value: " << values[i] << std::endl;
    }

    Game g;
    g.run();

    return 0;
}
