/* date = July 29th 2021 1:10 pm */

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

#define ARENA_RESERVE_SIZE Gigabytes(1)
#define ARENA_COMMIT_BLOCK_SIZE Megabytes(64)

typedef void* MemoryArenaReserveFunc(u64 Size);
typedef void  MemoryArenaFreeFunc(void* Address, u64 Size);
typedef void  MemoryAreanaCommitFunc(void* Address, u64 Size);
typedef void  MemoryArenaDecommitFunc(void* Address, u64 Size);

struct memory_arena {
    u8* Memory;
    u64 Cap;
    u64 At;
    u64 CommitAt;
};

static memory_arena ArenaMake();
static void ArenaRelease(memory_arena* Arena);
static void* ArenaPush(memory_arena* Arena, u64 Size);
static void ArenaPopTo(memory_arena* Arena, u64 PopTo);
#endif //MEMORY_ARENA_H
