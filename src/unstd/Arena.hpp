#pragma once

#include "unstd/types.hpp"
#include "unstd/DynamicArray.hpp"

namespace {

struct Arena {
private:
    u8* start;
    u8* end;
    u8* tip;

    using Self = Arena;

    template <typename T>
    u8* find_aligned_tip() const {
        u8* requested_memory = this->tip;

        while (requested_memory != this->end) {
            const std::uintptr_t iptr = reinterpret_cast<std::uintptr_t>(requested_memory);
            if (iptr % alignof(T) == 0) break;
            requested_memory++;
        }

        if (requested_memory == this->end) {
            return nullptr;
        } else {
            return requested_memory;
        }
    }

public:
    static Self init(u64 bytes) {
        u8* new_memory = memalloc<u8>(size_bytes);

        return Arena {
            .start = new_memory,
            .end = new_memory != nullptr ? new_memory + size_bytes : nullptr,
            .tip = new_memory
        };
    }

    void destroy(void) {
        free(this->start);
        this->start = nullptr;
        this->end   = nullptr;
        this->tip   = nullptr;
    }


    template <typename T>
    u8* allocate(u64 requested_count) {
        const u64 bytes_requested = sizeof(T) * requested_count;

        u8* memory_start = buffer.find_aligned_tip<T>();
        if (!memory_start) return nullptr;

        const u64 possible_bytes_left = buffer.end - memory_start;

        if (possible_bytes_left >= bytes_requested) {
            buffer.tip = memory_start + bytes_requested;
            return memory_start;
        } else {
            return nullptr;
        }
    }
};

}

struct ArenaAllocator {

private:
    DynamicArray<Arena> arenas;
    const u64 buffer_size;

public:
    using Self = ArenaAllocator;

    static Self init(u64 requested_buffer_size) {
        return Arena {
            .arenas = DynamicArray<Arena>::init(1),
            .buffer_size = requested_buffer_size
        };
    }

    void destroy(void) {
        for (Arena& b : this->arenas) b.destroy();
        this->arenas.destroy();
    }

    template <typename T>
    u8* allocate(u64 requested_count) {
        if (sizeof(T) * requested_count > this->buffer_size) return nullptr;

        for (Arena& buffer : this->arenas) {
            u8* potential_memory = buffer.allocate<T>(requested_count);
            if (potential_memory) return potential_memory;
        }

        this->arenas.append(Arena::init(this->buffer_size));
        return this->arenas.back().allocate<T>(requested_count);
    }
};

