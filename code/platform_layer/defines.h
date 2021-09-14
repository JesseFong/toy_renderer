/* date = July 29th 2020 1:47 pm */

#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
typedef int32_t   b32;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef float     f32;
typedef double    f64; 

typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef size_t    usize;
typedef ptrdiff_t isize;

typedef uintptr_t umm;
typedef intptr_t  smm;

#define Flag8(type) u8
#define Flag16(type) u16
#define Flag32(type) u32
#define Flag64(type) u64

#define Enum8(type) u8
#define Enum16(type) u16
#define Enum32(type) u32
#define Enum64(type) u64

#define S8MIN  ((s8)0x80)
#define S8MAX  ((s8)0x7f)
#define S16MIN ((s32)0x8000)
#define S16MAX ((s32)0x7fff)
#define S32MIN ((s32)0x80000000)
#define S32MAX ((s32)0x7fffffff) 
#define S64MIN ((s64)0x800000000000)
#define S64MAX ((s64)0x7fffffffffff) 

#define U8MAX  ((u8)-1)
#define U16MAX ((u16)-1)
#define U32MAX ((u32)-1)
#define U64MAX ((u64)-1)

#define F32MAX FLT_MAX
#define F32MIN -FLT_MAX

#define F64MAX DBL_MAX
#define F64MIN -DBL_MAX

#define PI32 3.14159265359f
#define TAU32 6.283185307179f

#define local_persist   static
#define global_variable static

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define SecondsToMS(Value) ((Value)*1000)
#define MinutesToMS(Value) (SecondsToMS(Value)*1000)
#define HoursToMS(Value)   (MinToMS(Value)*60)
#define DaysToMS(Value)    (HoursToMS(Value)*24)

#define U32FromPointer(Pointer)     ((u32)(usize)(Pointer))
#define PointerFromU32(type, Value) (type *)((usize)Value)

#define OffsetOf(type, Member) (umm)&(((type *)0)->Member)

#define FILE_AND_LINE__(A, B) A "|" #B
#define FILE_AND_LINE_(A, B) FILE_AND_LINE__(A, B)
#define FILE_AND_LINE FILE_AND_LINE_(__FILE__, __LINE__)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define ClearArrayCount(Array, Count) for(u32 i = 0; i < Count; i++) {Array[i] = {};}
#define ClearArray(Array) ClearArrayCount(Array, ArrayCount(Array))
#define ZeroStruct(a) memset(&(a), 0, sizeof(a))

#define Min(A, B) (A < B ? A : B)
#define Max(A, B) (A > B ? A : B)
#define ClampTop(A, B) Min(A, B)
#define ClampBot(A, B) Max(A, B)

#define AlignPow2(X, P) (((X) + (P) - 1) & ~((P) - 1))

#define NotImplemented Assert(!"NotImplemented")
#define NotSupposedToBeHere Assert(!"Not Supposed To Be Here")
#define InvalidDefaultCase default: {NotSupposedToBeHere;} break

#define DEBUG_BREAK() __asm int 3

#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;} 
#define AssertMessage(Condition, Message, ...) do { \
if (!(Condition)) { \
AssertHandler("Assertion Failure", #Condition, __FILE__, cast(i64)__LINE__, Message, ##__VA_ARGS__); \
GB_DEBUG_TRAP(); \
} \
} while (0)
static void AssertHandler(char const *Prefix, char const *Condition, char const *File, s32 Line, char const *Message, ...);



inline u32
RotateLeft(u32 Value, s32 Amount) {
#if COMPILER_MSVC
    u32 Result = _rotl(Value, Amount);
#else
    Amount &= 31;
    u32 Result = ((Value << Amount) | (Value >> (32 - Amount)));
#endif
    
    return(Result);
}

inline u64
RotateLeft(u64 Value, s32 Amount)
{
#if COMPILER_MSVC
    u64 Result = _rotl64(Value, Amount);
#else
    Amount &= 63;
    u64 Result = ((Value << Amount) | (Value >> (64 - Amount)));
#endif
    
    return(Result);
}

inline u32
RotateRight(u32 Value, s32 Amount) {
#if COMPILER_MSVC
    u32 Result = _rotr(Value, Amount);
#else
    Amount &= 31;
    u32 Result = ((Value >> Amount) | (Value << (32 - Amount)));
#endif
    
    return(Result);
}

static u8
SafeTruncateToU8(u16 Value){Assert(Value <= U8MAX);u8 Result = (u8)Value;return(Result);}
static u8
SafeTruncateToU8(u32 Value){Assert(Value <= U8MAX);u8 Result = (u8)Value;return(Result);}
static u8
SafeTruncateToU8(u64 Value){Assert(Value <= U8MAX);u8 Result = (u8)Value;return(Result);}
static u16
SafeTruncateToU16(u32 Value){Assert(Value <= U16MAX);u16 Result = (u16)Value;return(Result);}  
static u16
SafeTruncateToU16(u64 Value){Assert(Value <= U16MAX);u16 Result = (u16)Value;return(Result);}  
static u32
SafeTruncateToU32(u64 Value){Assert(Value <= U32MAX);u32 Result = (u32)Value;return(Result);}


struct bit_scan_result {
    b32 Found;
    u32 Index;
};

inline bit_scan_result
FindLeastSignificantSetBit(u32 Value) {
    bit_scan_result Result = {};
#if COMPILER_MSVC
    Result.Found = _BitScanForward((unsigned long *)&Result.Index, Value);
#else
    for(u32 Test = 0;
        Test < 32;
        ++Test) {
        if(Value & (1 << Test)) {
            Result.Index = Test;
            Result.Found = true;
            break;
        }
    }
#endif
    return(Result);
}

//
// Print functions
//
//

static s32 InternalVSNPrintf(char* String, usize Size, char* Format, va_list VAList) {
    s32 Result;
    Result = vsnprintf(String, Size, Format, VAList);
    if(Size) {
        String[Size - 1] = 0;
    }
    
    
    return (Result >= (s32)Size || Result < 0) ? -1 : Result;
}

static char* FormatString(char* Format, ...) {
    local_persist char Buffer[2048];
    va_list VAList;
    va_start(VAList, Format);
    InternalVSNPrintf(Buffer, 2048, Format, VAList);
    va_end(VAList);
    return(Buffer);
}

//
// RNG: Random Number Generator
//
// PCG generatator based on https://www.pcg-random.org/
//

struct random_series_pcg {
    u64 State;
    u64 Selector; //Must be odd
};

static random_series_pcg
GetRandomSeries(u64 State, u64 Selector) {
    random_series_pcg Result;
    Result.State = State;
    Result.Selector = (Selector << 1) | 1;
}

static random_series_pcg
GetDefaultRandomSeries() {
    random_series_pcg Result = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};
    return(Result);
}

static u32
RandomU32(random_series_pcg* Series) {
    u64 State = Series->State;
    State = State * 6364136223846793005ULL + Series->Selector;
    Series->State = State;
    
    u32 PreRotate = (u32)((State ^ (State >> 18)) >> 27);
    u32 Result = RotateRight(PreRotate, (s32)(State >> 59));
    return(Result);
}

static f32
RandomUnilateral(random_series_pcg* Series) {
    f32 Divisor = 1.0f / (f32)U32MAX;
    f32 Result = Divisor*(f32)RandomU32(Series);
    return(Result);
}

static f32
RandomBilateral(random_series_pcg* Series) {
    f32 Result = 2.0f*RandomUnilateral(Series) - 1.0f;
	return Result;
}

static u32
RandomBoundFast(random_series_pcg* Series, u32 Bound) {
    u32 Result = RandomU32(Series);
    return(Result % Bound);
}

#endif //DEFINES_H

