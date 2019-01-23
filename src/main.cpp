#include <SDL.h>
#include <time.h>
#include "Game.hpp"
#include <iostream>
#include <assert.h>

#include "unstd/DynamicArray.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/DenseHashTable.hpp"

struct IntHasher {
    static int hash(int x) { return x; }
};

int main(int argc, char** argv) {
    srand (static_cast<unsigned>(time(nullptr)));

    DenseHashTable<int, int, IntHasher> table(0,1);

    insert(table, 10, 20);
    auto x = lookup(table, 10);
    std::cout << *x << std::endl;

    insert(table, 31, 69);
    insert(table, 32, 69);
    insert(table, 11, 12);

	for_each(table, [](int k, int v) {
		printf("%d %d\n", k, v);
	});

    DynamicArray<int> arr;
    reserve(arr, 100);
    append(arr, 3);
    append(arr, 4);
    append(arr, 5);

	printf("arr size: %d\n", arr.size);
	for_each(arr, [](auto i) {
		printf("%d\n", i);
	});

    clear(arr);
	printf("arr size: %d\n", arr.size);
	for_each(arr, [](auto i) {
		printf("%d\n", i);
	});

	for_each(arr, [](auto i) {
		printf("%d\n", i);
	});

    ArraySet<int> aset;
    reserve(aset, 100);

    insert(aset, 2);
    insert(aset, -1);
    insert(aset, 333);
    insert(aset, -10);
    insert(aset, 9999);
    insert(aset, 34);

    printf("set size: %d\n", size(aset));

	for_each(aset, [](auto i) {
		printf("set: %d\n", i);
	});

    remove(aset, -10);
    remove(aset, 9999);
    remove(aset, 123456);

    printf("set size: %d\n", size(aset));

	for_each(aset, [](auto i) {
		printf("set: %d\n", i);
	});

    Game g;
    g.run();

    return 0;
}
