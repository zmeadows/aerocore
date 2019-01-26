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

    DenseHashTable<int,int,IntHasher> hash_table(100, 0, 1);

    for (int i = 2; i < 1e5; i++) {
        insert(hash_table, i, i*2);
    }

    for (int i = 2; i < 1e5; i++) {
        assert(*lookup(hash_table, i) == i*2);
    }

    for (int i = 2; i < 1e5; i++) {
        remove(hash_table, i);
    }

    std::cout << "TABLE SIZE: " << hash_table.size << std::endl;

    Game g;
    g.run();

    return 0;
}
