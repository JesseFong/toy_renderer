
static memory_arena
ArenaMake() {
    memory_arena Result = {};
    Result.Memory = (u8*)PlatformReserveMemory(ARENA_RESERVE_SIZE);
    Result.Cap = ARENA_RESERVE_SIZE;
    Result.At = 0;
    Result.CommitAt = 0;
    return Result;
}

static void
ArenaRelease(memory_arena* Arena) {
    PlatformFreeMemory(Arena->Memory, Arena->Cap);
}

static void*
ArenaPush(memory_arena* Arena, u64 Size) {
    void* Result = 0;
    
    if(Arena->At + Size <= Arena->Cap) {
        Result = Arena->Memory + Arena->At;
        Arena->At += Size;
        
        if(Arena->At > Arena->CommitAt) {
            u64 NextCommitAt = AlignPow2(Arena->At, ARENA_COMMIT_BLOCK_SIZE - 1);
            u64 NextCommitAtClamped = ClampTop(NextCommitAt, Arena->Cap);
            u64 CommitSize = NextCommitAtClamped - Arena->CommitAt;
            
            PlatformCommitMemory(Arena->Memory + Arena->CommitAt, CommitSize);
            Arena->CommitAt = NextCommitAtClamped;
        }
    }
    return Result;
}

static void
ArenaPopTo(memory_arena* Arena, u64 PopTo) {
    if(PopTo < Arena->At) {
        Arena->At = PopTo;
        
        u64 AtAligned = AlignPow2(Arena->At, ARENA_COMMIT_BLOCK_SIZE - 1);
        u64 NextCommitAt = ClampTop(AtAligned, Arena->Cap);
        
        if(NextCommitAt < Arena->CommitAt) {
            u64 DecommitSize = Arena->CommitAt - NextCommitAt;
            PlatformDecommitMemory(Arena->Memory + NextCommitAt, DecommitSize);
            Arena->CommitAt = NextCommitAt;
        }
    }
}

#define ArenaPushStruct(Arena, type) (type*)ArenaPush(Arena, sizeof(type))
#define ArenaPushArray(Arena, Count, type) (type*)ArenaPush(Arena, (Count)*sizeof(type))

static void*
ArenaPushSizePlusNull(memory_arena* Arena, u64 Size) {
    Size += 1;
    void* Result = ArenaPush(Arena, Size);
    u8* NullLocation = (u8*)Result + (Size - 1);
    *NullLocation = 0;
    return Result;
}
