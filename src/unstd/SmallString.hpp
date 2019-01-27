#pragma once

#define SMALL_STRING_CAPACITY = 22;

struct SmallString {
    char[SMALL_STRING_CAPACITY] data;
    u32 size;
};
