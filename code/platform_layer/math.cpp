#include <math.h>
#include "math.h"

inline f32
Radians(f32 D) {
    f32 Result = (f32)(D * 0.017453292);
    return Result;
}

inline f32
Degrees(f32 R) {
    f32 Result = (f32)((R)*(180/PI32));
    return Result;
}

inline s32
SignOf(s32 Value) {
    s32 Result = (Value >= 0) ? 1 : -1;
    return Result;
}

inline f32
SignOf(f32 Value) {
    f32 Result = (Value >= 0) ? 1.0f : -1.0f;
    return Result;
}

inline f32
AbsoluteValue(f32 F32) {
    f32 Result = (f32)fabs(F32);
    return Result;
}

//
// Rounding Ceil Floor
//

inline f32
Floor(f32 F32) {
    f32 Result = floorf(F32);
    return Result;
}

inline f32
Ceil(f32 F32) {
    f32 Result = ceilf(F32);
    return Result;
}

inline s32
RoundF32ToS32(f32 F32) {
    s32 Result = (s32)roundf(F32);
    return Result;
}

inline u32
RoundF32ToU32(f32 F32) {
    u32 Result = (u32)roundf(F32);
    return Result;
}

inline u32
FloorF32ToU32(f32 F32) {
    u32 Result = (u32)floorf(F32);
    return Result;
}

inline s32 
FloorF32ToS32(f32 F32) {
    s32 Result = (s32)floorf(F32);
    return Result;
}

inline s32 
CeilF32ToS32(f32 F32) {
    s32 Result = (s32)ceilf(F32);
    return Result;
}

inline s32
TruncateF32ToS32(f32 F32) {
    s32 Result = (s32)F32;
    return Result;
}


//
// Trigonometry functions
//

inline f32
Sin(f32 Angle) {
    f32 Result = sinf(Angle);
    return Result;
}

inline f32
Sin01(f32 t) {
    f32 Result = Sin(PI32*t);
    
    return Result;
}

inline f32
Cos(f32 Angle) {
    f32 Result = cosf(Angle);
    return Result;
}

inline f32
Tan(f32 Angle) {
    f32 Result = tanf(Angle);
    return Result;
}

inline f32
ATan(f32 Distance) {
    f32 Result = atanf(Distance);
    return Result;
}

inline f32
ATan2(f32 Y, f32 X) {
    f32 Result = atan2f(Y, X);
    return Result;
}

inline f32
Triangle01(f32 t) {
    f32 Result = 2.0f*t;
    if(Result > 1.0f) {
        Result = 2.0f - Result;
    }
    return Result;
}

inline f32
AngleFromCenterOfDisk(f32 A, f32 B, f32 C) {
    f32 Result = 0;
    f32 Tolerance = 0.0001f;
    f32 Denom = (-2*A*B);
    if(AbsoluteValue(Denom) > Tolerance) {
        Result = (-(A*A) + (C*C) - (B*B))/Denom;
    }
    return Result;
}
//
// Scalar operations
//


inline f32
SquareRoot(f32 F32) {
    f32 Result = sqrtf(F32);
    return Result;
}

inline f32
ReciprocalSquareRoot(f32 Real32)
{
    f32 Result = (1.0f / SquareRoot(Real32));
    return Result;
}

inline f32
Pow(f32 A, f32 B) {
    f32 Result = (f32)pow(A, B);
    return Result;
}

inline f32
Square(f32 A) {
    f32 Result = A*A;
    
    return Result;
}

inline f32
Lerp(f32 A, f32 t, f32 B) {
    f32 Result = (1.0f - t)*A + t*B;
    return Result;
}

inline f32
Clamp(f32 Min, f32 Value, f32 Max) {
    f32 Result = Value;
    if(Result < Min) {
        Result = Min;
    }
    else if(Result > Max) {
        Result = Max;
    }
    return Result;
} 

inline f32
Clamp01(f32 Value) {
    f32 Result = Clamp(0.0f, Value, 1.0f);
    return Result;
}

inline f32
Clamp01MapToRange(f32 Min, f32 t, f32 Max) {
    f32 Result = 0.0f;
    f32 Range = Max - Min;
    if(Range != 0.0f) {
        Result = Clamp01((t - Min) / Range);
    }
    return Result;
}

inline f32
ClampAboveZero(f32 Value) {
    f32 Result = (Value < 0) ? 0.0f : Value;
    return Result;
}

inline f32
SafeRatioN(f32 Numerator, f32 Divisor, f32 N) {
    f32 Result = N;
    if(Divisor != 0.0f) {
        Result = Numerator / Divisor;
    }
    return Result;
}

inline f32
SafeRatio0(f32 Numerator, f32 Divisor) {
    f32 Result = SafeRatioN(Numerator, Divisor, 0.0f);
    return Result;
}

inline f32
SafeRatio1(f32 Numerator, f32 Divisor) {
    f32 Result = SafeRatioN(Numerator, Divisor, 1.0f);
    return Result;
}

inline f64
SafeRatioN(f64 Numerator, f64 Divisor, f32 N) {
    f64 Result = N;
    if(Divisor != 0.0f) {
        Result = Numerator / Divisor;
    }
    return Result;
}

inline f64
SafeRatio0(f64 Numerator, f64 Divisor) {
    f64 Result = SafeRatioN(Numerator, Divisor, 0.0);
    return Result;
}

inline f64
SafeRatio1(f64 Numerator, f64 Divisor) {
    f64 Result = SafeRatioN(Numerator, Divisor, 1.0f);
    return Result;
}

inline f32
LinearConversion(f32 OldMin, f32 OldMax, f32 NewMin, f32 NewMax, f32 t) {
    f32 Result = SafeRatio0((t - OldMin), (OldMax - OldMin));
    Result = Result * (NewMax - NewMin) + NewMin;
    return Result;
}
//
// v2 operations
//

inline v2
operator+(v2 A, v2 B) {
    v2 Result = {(A.x + B.x), (A.y + B.y)};
    return Result;
}

inline v2 &
operator+=(v2 &A, v2 B) {
    A = A + B;
    return A;
}

inline v2
operator-(v2 A) {
    v2 Result = {-A.x, -A.y};
    return Result;
}

inline v2
operator-(v2 A, v2 B) {
    v2 Result = {(A.x - B.x), (A.y - B.y)};
    return Result;
}

inline v2 &
operator-=(v2 &A, v2 B) {
    A = A - B;
    return A;
}

inline v2
operator*(f32 A, v2 B) {
    v2 Result = {(A*B.x), (A*B.y)};
    return Result;
}

inline v2
operator*(v2 A, f32 B) {
    v2 Result = {(A.x*B), (A.y*B)};
    return Result;
}

inline v2 &
operator*=(v2 &B, f32 A) {
    B = A * B;
    return B;
}

inline v2
operator/(v2 A, f32 B) {
    v2 Result = (1.0f/B)*A;
    return Result;
}

inline v2 &
operator/=(v2 &A, f32 B) {
    A = A / B;
    return A;
}


inline v2
Hadamard(v2 A, v2 B) {
    v2 Result = {A.x*B.x, A.y*B.y};
    return Result;
}

inline v2
operator*(v2 A, v2 B) {
    v2 Result = Hadamard(A, B);
    return Result;
}

inline v2 &
operator*=(v2 &B, v2 A) {
    B = A * B;
    return B;
}

inline v2
Perp(v2 A) {
    v2 Result = {-A.y, A.x};
    return Result;
}


inline f32
Inner(v2 A, v2 B) {
    f32 Result = A.x*B.x + A.y*B.y;
    return Result;
}

inline f32
LengthSq(v2 A) {
    f32 Result = Inner(A, A);
    return Result;
}

inline f32
Length(v2 A) {
    f32 Result = SquareRoot(LengthSq(A));
    return Result;
}

inline v2
Normalize(v2 A) {
    v2 Result = A * (1.0f / Length(A));
    return Result;
}

inline v2
NOZ(v2 A) {
    v2 Result = {};
    f32 LenSq = LengthSq(A);
    if(LenSq > Square(0.0001f)) {
        Result = A * (1.0f / SquareRoot(LenSq));
    }
    return Result;
}

inline v2
Clamp(v2 Min, v2 Value, v2 Max) {
    v2 Result;
    Result.x = Clamp(Min.x, Value.x, Max.x);
    Result.y = Clamp(Min.y, Value.y, Max.y);
    return Result;
} 


inline v2
Clamp01(v2 Value) {
    v2 Result;
    Result.x = Clamp01(Value.x);
    Result.y = Clamp01(Value.y);
    return Result;
}


inline v2
Lerp(v2 A, f32 t, v2 B) {
    v2 Result = (1.0f - t)*A + t*B;
    return Result;
}

inline v2
Arm(f32 Angle) {
    v2 Result = {Cos(Angle), Sin(Angle)};
    return Result;
}


inline v2u
operator+(v2u A, v2u B) {
    v2u Result = {(A.x + B.x), (A.y + B.y)};
    return Result;
}

inline v2u &
operator+=(v2u &A, v2u B) {
    A = A + B;
    return A;
}

inline v2u
operator-(v2u A, v2u B) {
    v2u Result = {(A.x - B.x), (A.y - B.y)};
    return Result;
}

inline v2u &
operator-=(v2u &A, v2u B) {
    A = A - B;
    return A;
}

inline v2u
operator*(u32 A, v2u B) {
    v2u Result = {(A*B.x), (A*B.y)};
    return Result;
}

inline v2u
operator*(v2u A, u32 B) {
    v2u Result = A*B;
    return Result;
}

inline v2u &
operator*=(v2u &B, u32 A) {
    B = A * B;
    return B;
}

inline v2u
operator*(v2u A, v2u B) {
    v2u Result = {(A.x*B.x), (A.y*B.y)};
}

inline v2s
operator+(v2s A, v2s B) {
    v2s Result = {(A.x + B.x), (A.y + B.y)};
    return Result;
}

inline v2s &
operator+=(v2s &A, v2s B) {
    A = A + B;
    return A;
}

inline v2s
operator-(v2s A) {
    v2s Result = {-A.x, -A.y};
    return Result;
}

inline v2s
operator-(v2s A, v2s B) {
    v2s Result = {(A.x - B.x), (A.y - B.y)};
    return Result;
}

inline v2s &
operator-=(v2s &A, v2s B) {
    A = A - B;
    return A;
}

inline v2s
operator*(f32 A, v2s B) {
    v2s Result = {(A*B.x), (A*B.y)};
    return Result;
}

inline v2s
operator*(v2s A, f32 B) {
    v2s Result = A*B;
    return Result;
}

inline v2s &
operator*=(v2s &B, f32 A) {
    B = A * B;
    return B;
}


//
// v3 Operations
//

inline v3
operator+(v3 A, v3 B) {
    v3 Result = {(A.x + B.x), (A.y + B.y), (A.z + B.z)};
    return Result;
} 

inline v3 &
operator+=(v3 &A, v3 B) {
    A = A + B;
    return A;
}

inline v3
operator-(v3 A) {
    v3 Result = {-A.x, -A.y, -A.z};
    return Result;
}

inline v3
operator-(v3 A, v3 B) {
    v3 Result = {(A.x - B.x), (A.y - B.y), (A.z - B.z)};
    return Result;
} 

inline v3 
operator-=(v3 &A, v3 B) {
    A = A - B;
    return A;
}

inline v3
operator*(f32 A, v3 B) {
    v3 Result = {(A*B.x), (A*B.y), (A*B.z)};
    return Result;
}

inline v3
operator*(v3 B, f32 A) {
    v3 Result = A*B;
    return Result;
}

inline v3 &
operator*=(v3 &B, f32 A) {
    B = A * B;
    return B;
}

inline v3
operator/(v3 B, f32 A) {
    v3 Result = (1.0f/A)*B;
    return Result;
}

inline v3 &
operator/=(v3 &B, f32 A) {
    B = B / A;
    return B;
}

inline v3
Hadamard(v3 A, v3 B) {
    v3 Result = {A.x*B.x, A.y*B.y, A.z*B.z};
    return Result;
}

inline f32
Inner(v3 A, v3 B) {
    f32 Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return Result;
}

inline v3
Cross(v3 A, v3 B) {
    v3 Result;
    Result.x = A.y*B.z - A.z*B.y;
    Result.y = A.z*B.x - A.x*B.z;
    Result.z = A.x*B.y - A.y*B.x;
    return Result;
}

inline f32
LengthSq(v3 A) {
    f32 Result = Inner(A, A);
    return Result;
}

inline f32
Length(v3 A) {
    f32 Result = SquareRoot(LengthSq(A));
    return Result;
}

inline v3
Normalize(v3 A) {
    v3 Result = A * (1.0f / Length(A));
    return Result;
}

inline v3
NOZ(v3 A) {
    v3 Result = {};
    f32 LenSq = LengthSq(A);
    if(LenSq > Square(0.0001f)) {
        Result = A * (1.0f / SquareRoot(LenSq));
    }
    return Result;
}

inline v3
Clamp01(v3 Value) {
    v3 Result;
    Result.x = Clamp01(Value.x);
    Result.y = Clamp01(Value.y);
    Result.z = Clamp01(Value.z);
    return Result;
}

inline v3
Lerp(v3 A, f32 t, v3 B) {
    v3 Result = (1.0f - t)*A + t*B;
    return Result;
}

//
// v4 operations
//

inline v4
operator+(v4 A, v4 B) {
    v4 Result = {(A.x + B.x), (A.y + B.y), (A.z + B.z), (A.w + B.w)};
    return Result;
} 

inline v4 &
operator+=(v4 &A, v4 B) {
    A = A + B;
    return A;
}

inline v4
operator-(v4 A) {
    v4 Result = {-A.x, -A.y, -A.z, -A.w};
    return Result;
}

inline v4
operator-(v4 A, v4 B) {
    v4 Result = {(A.x - B.x), (A.y - B.y), (A.z - B.z), (A.w - B.w)};
    return Result;
} 

inline v4 &
operator-=(v4 &A, v4 B) {
    A = A - B;
    return(A);
}

inline v4
operator*(f32 A, v4 B) {
    v4 Result = {(A*B.x), (A*B.y), (A*B.z), (A*B.w)};
    return Result;
}

inline v4
operator*(v4 B, f32 A) {
    v4 Result = A*B;
    return Result;
}

inline v4 &
operator*=(v4 &B, f32 A) {
    B = A * B;
    return B;
}

inline v4
operator/(v4 B, f32 A) {
    v4 Result = (1.0f/A)*B;
    return Result;
}

inline v4 &
operator/=(v4 &B, f32 A) {
    B = B / A;
    return B;
}

inline v4
Hadamard(v4 A, v4 B) {
    v4 Result = {A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w};
    return Result;
}

inline f32
Inner(v4 A, v4 B) {
    f32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
    return Result;
}

inline f32
LengthSq(v4 A) {
    f32 Result = Inner(A, A);
    return Result;
}

inline f32
Length(v4 A) {
    f32 Result = SquareRoot(LengthSq(A));
    return Result;
}

inline v4
Normalize(v4 A) {
    v4 Result = A * (1.0f / Length(A));
    return Result;
}

inline v4
NOZ(v4 A) {
    v4 Result = {};
    f32 LenSq = LengthSq(A);
    if(LenSq > Square(0.0001f)) {
        Result = A * (1.0f / SquareRoot(LenSq));
    }
    return Result;
}

inline v4
Clamp01(v4 Value) {
    v4 Result;
    Result.x = Clamp01(Value.x);
    Result.y = Clamp01(Value.y);
    Result.z = Clamp01(Value.z);
    return Result;
}

inline v4
Lerp(v4 A, f32 t, v4 B) {
    v4 Result = (1.0f - t)*A + t*B;
    return Result;
}

//
// rectangle2 opersions
//


inline f32
GetWidth(rectangle2 A) {
    f32 Result = A.Max.x - A.Min.x;
    return Result;
}

inline f32
GetHeight(rectangle2 A) {
    f32 Result = A.Max.y - A.Min.y;
    return Result;
}

inline rectangle2
InvertedInfinityRectangle2(void) {
    rectangle2 Result;
    Result.Min.x = Result.Min.y = F32MAX;
    Result.Max.x = Result.Max.y = F32MIN;
    return Result;
}

inline rectangle2
Union(rectangle2 A, rectangle2 B) {
    rectangle2 Result;
    Result.Min.x = (A.Min.x < B.Min.x) ? A.Min.x : B.Min.x;
    Result.Min.y = (A.Min.y < B.Min.y) ? A.Min.y : B.Min.y;
    Result.Max.x = (A.Max.x > B.Max.x) ? A.Max.x : B.Max.x;
    Result.Max.y = (A.Max.y > B.Max.y) ? A.Max.y : B.Max.y;
    return Result;
}

inline v2
GetMinCorner(rectangle2 Rect) {
    v2 Result = Rect.Min;
    return Result;
}

inline v2
GetMaxCorner(rectangle2 Rect) {
    v2 Result = Rect.Max;
    return Result;
}

inline v2
GetDim(rectangle2 Rect) {
    v2 Result = Rect.Max - Rect.Min;
    return Result;
}

inline v2
GetCenter(rectangle2 Rect) {
    v2 Result = 0.5f*(Rect.Min + Rect.Max);
    return Result;
}

inline rectangle2
RectMinMax(v2 Min, v2 Max) {
    rectangle2 Result;
    Result.Min = Min;
    Result.Max = Max;
    return Result;
}

inline rectangle2
RectMinMax(f32 MinX, f32 MinY, f32 MaxX, f32 MaxY) {
    rectangle2 Result;
    Result.Min = V2(MinX, MinY);
    Result.Max = V2(MaxX, MaxY);
    return Result;
}

inline rectangle2
RectMinDim(v2 Min, v2 Dim) {
    rectangle2 Result;
    Result.Min = Min;
    Result.Max = Min + Dim;
    return Result;
}

inline rectangle2
RectCenterHalfDim(v2 Center, v2 HalfDim) {
    rectangle2 Result;
    Result.Min = Center - HalfDim;
    Result.Max = Center + HalfDim;
    return Result;
}

inline rectangle2
AddRadiusTo(rectangle2 A, v2 Radius) {
    rectangle2 Result;
    Result.Min = A.Min - Radius;
    Result.Max = A.Max + Radius;
    return Result;
}

inline rectangle2
Offset(rectangle2 A, v2 Offset) {
    rectangle2 Result;
    Result.Min = A.Min + Offset;
    Result.Max = A.Max + Offset;
    return Result;
}

inline rectangle2
RectCenterDim(v2 Center, v2 Dim) {
    rectangle2 Result = RectCenterHalfDim(Center, 0.5f*Dim);
    
    return Result;
}

inline b32
IsInRectangle(rectangle2 Rectangle, v2 Test) {
    if(Rectangle.Min.x > Rectangle.Max.x) {
        f32 Temp = Rectangle.Min.x;
        Rectangle.Min.x = Rectangle.Max.x;
        Rectangle.Max.x = Temp; 
    }
    if(Rectangle.Min.y > Rectangle.Max.y) {
        f32 Temp = Rectangle.Min.y;
        Rectangle.Min.y = Rectangle.Max.y;
        Rectangle.Max.y = Temp; 
    }
    b32 Result = ((Test.x >= Rectangle.Min.x) &&
                  (Test.y >= Rectangle.Min.y) &&
                  (Test.x < Rectangle.Max.x) &&
                  (Test.y < Rectangle.Max.y));
    
    return Result;
}

inline b32
RectanglesIntersect(rectangle2 A, rectangle2 B) {
    b32 Result = !((B.Max.x <= A.Min.x) ||
                   (B.Min.x >= A.Max.x) ||
                   (B.Max.y <= A.Min.y) ||
                   (B.Min.y >= A.Max.y));
    return Result;
}

inline v2
GetBarycentric(rectangle2 A, v2 P) {
    v2 Result;
    
    Result.x = SafeRatio0(P.x - A.Min.x, A.Max.x - A.Min.x);
    Result.y = SafeRatio0(P.y - A.Min.y, A.Max.y - A.Min.y);
    
    return Result;
}

inline f32
GetArea(rectangle2 A) {
    v2 Dim = GetDim(A);
    f32 Result = Dim.x*Dim.y;
    return Result;
}


//
// NOTE(Jesse): Rectangle3
//
inline rectangle2
Rect3To2(rectangle3 Rect) {
    rectangle2 Result;
    Result.Min = Rect.Min.xy;
    Result.Max = Rect.Max.xy;
    return Result;
}

inline v3
GetMinCorner(rectangle3 Rect) {
    v3 Result = Rect.Min;
    return Result;
}

inline v3
GetMaxCorner(rectangle3 Rect) {
    v3 Result = Rect.Max;
    return Result;
}

inline v3
GetDim(rectangle3 Rect) {
    v3 Result = Rect.Max - Rect.Min;
    return Result;
}

inline v3
GetCenter(rectangle3 Rect) {
    v3 Result = 0.5f*(Rect.Min + Rect.Max);
    return Result;
}

inline rectangle3
InvertedInfinityRectangle3() {
    rectangle3 Result = {};
    Result.Min.x = Result.Min.y = Result.Min.z = F32MAX;
    Result.Max.x = Result.Max.y = Result.Max.z = F32MIN;
    return Result;
}

inline rectangle3
RectMinMax(v3 Min, v3 Max) {
    rectangle3 Result;
    
    Result.Min = Min;
    Result.Max = Max;
    
    return Result;
}

inline rectangle3
RectMinDim(v3 Min, v3 Dim) {
    rectangle3 Result;
    
    Result.Min = Min;
    Result.Max = Min + Dim;
    
    return Result;
}

inline rectangle3
RectCenterHalfDim(v3 Center, v3 HalfDim) {
    rectangle3 Result;
    
    Result.Min = Center - HalfDim;
    Result.Max = Center + HalfDim;
    
    return Result;
}

inline rectangle3
AddRadiusTo(rectangle3 A, v3 Radius) {
    rectangle3 Result;
    
    Result.Min = A.Min - Radius;
    Result.Max = A.Max + Radius;
    
    return Result;
}

inline rectangle3
Offset(rectangle3 A, v3 Offset) {
    rectangle3 Result;
    
    Result.Min = A.Min + Offset;
    Result.Max = A.Max + Offset;
    
    return Result;
}

inline rectangle3
RectCenterDim(v3 Center, v3 Dim) {
    rectangle3 Result = RectCenterHalfDim(Center, 0.5f*Dim);
    
    return Result;
}

inline rectangle3
UnionPoint(rectangle3 Rect, v3 Point) {
    rectangle3 Result = {};
    
    Result.Min.x = (Point.x < Rect.Min.x) ? Point.x : Rect.Min.x;
    Result.Max.x = (Point.x > Rect.Max.x) ? Point.x : Rect.Max.x;
    Result.Min.y = (Point.y < Rect.Min.y) ? Point.y : Rect.Min.y;
    Result.Max.y = (Point.y > Rect.Max.y) ? Point.y : Rect.Max.y;
    Result.Min.z = (Point.z < Rect.Min.z) ? Point.z : Rect.Min.z;
    Result.Max.z = (Point.z > Rect.Max.z) ? Point.z : Rect.Max.z;
    
    return Result;
}

inline b32
IsInRectangle(rectangle3 Rectangle, v3 Test) {
    if(Rectangle.Min.x > Rectangle.Max.x) {
        f32 Temp = Rectangle.Min.x;
        Rectangle.Min.x = Rectangle.Max.x;
        Rectangle.Max.x = Temp; 
    }
    if(Rectangle.Min.y > Rectangle.Max.y) {
        f32 Temp = Rectangle.Min.y;
        Rectangle.Min.y = Rectangle.Max.y;
        Rectangle.Max.y = Temp; 
    }
    if(Rectangle.Min.z > Rectangle.Max.z) {
        f32 Temp = Rectangle.Min.z;
        Rectangle.Min.z = Rectangle.Max.z;
        Rectangle.Max.z = Temp; 
    }
    b32 Result = ((Test.x >= Rectangle.Min.x) &&
                  (Test.y >= Rectangle.Min.y) &&
                  (Test.z >= Rectangle.Min.z) &&
                  (Test.x < Rectangle.Max.x) &&
                  (Test.y < Rectangle.Max.y) &&
                  (Test.z < Rectangle.Max.z));
    
    return Result;
}

inline b32
RectanglesIntersect(rectangle3 A, rectangle3 B) {
    b32 Result = !((B.Max.x <= A.Min.x) ||
                   (B.Min.x >= A.Max.x) ||
                   (B.Max.y <= A.Min.y) ||
                   (B.Min.y >= A.Max.y) ||
                   (B.Max.z <= A.Min.z) ||
                   (B.Min.z >= A.Max.z));
    return Result;
}

inline v3
GetBarycentric(rectangle3 A, v3 P) {
    v3 Result;
    
    Result.x = SafeRatio0(P.x - A.Min.x, A.Max.x - A.Min.x);
    Result.y = SafeRatio0(P.y - A.Min.y, A.Max.y - A.Min.y);
    Result.z = SafeRatio0(P.z - A.Min.z, A.Max.z - A.Min.z);
    
    return Result;
}

inline f32
GetArea(rectangle3 A) {
    v3 Dim = GetDim(A);
    f32 Result = Dim.x*Dim.y*Dim.z;
    return Result;
}

//
// Color Operations
//

inline v4
LinearToRGBA255(v4 C) {
    v4 Result;
    
    f32 One255 = 255.0f;
    Result.r = One255*C.r;
    Result.g = One255*C.g;
    Result.b = One255*C.b;
    Result.a = One255*C.a;
    
    return Result;
}

inline v4
SRGBLinearize(v4 C) {
    v4 Result;
    
    Result.r = Square(C.r);
    Result.g = Square(C.g);
    Result.b = Square(C.b);
    Result.a = C.a;
    
    return Result;
}

inline v4
SRGBLinearize(f32 R, f32 G, f32 B, f32 A) {
    v4 Input = {R, G, B, A};
    v4 Result = SRGBLinearize(Input);
    return Result;
}

inline v4
SRGB255ToLinear1(v4 C) {
    v4 Result;
    
    f32 Inv255 = 1.0f / 255.0f;
    
    Result.r = Square(Inv255*C.r);
    Result.g = Square(Inv255*C.g);
    Result.b = Square(Inv255*C.b);
    Result.a = Inv255*C.a;
    
    return Result;
}

inline v4
Linear1ToSRGB255(v4 C) {
    v4 Result;
    
    f32 One255 = 255.0f;
    
    Result.r = One255*SquareRoot(C.r);
    Result.g = One255*SquareRoot(C.g);
    Result.b = One255*SquareRoot(C.b);
    Result.a = One255*C.a;
    
    return Result;
}

inline f32
RealLinear1ToSRGB(f32 L) {
    if(L < 0) {
        L = 0;
    }
    if(L > 1) {
        L = 1;
    }
    
    f32 Result = L*12.92f;
    if(L >= 0.0031308) {
        Result = (1.055f*Pow(L, 1.0f/2.4f) - 0.055f);
    }
    
    return Result;
}

inline v4
BGRAUnpack4x8(u32 Packed) {
    v4 Result = {(f32)((Packed >> 16) & 0xFF),
        (f32)((Packed >> 8) & 0xFF),
        (f32)((Packed >> 0) & 0xFF),
        (f32)((Packed >> 24) & 0xFF)};
    
    return Result;
}

inline u32
BGRAPack4x8(v4 Unpacked) {
    u32 Result = ((RoundF32ToU32(Unpacked.a) << 24) |
                  (RoundF32ToU32(Unpacked.r) << 16) |
                  (RoundF32ToU32(Unpacked.g) << 8) |
                  (RoundF32ToU32(Unpacked.b) << 0));
    
    return Result;
}

inline v4
RGBAUnpack4x8(u32 Packed) {
    v4 Result = {(f32)((Packed >> 0) & 0xFF),
        (f32)((Packed >> 8) & 0xFF),
        (f32)((Packed >> 16) & 0xFF),
        (f32)((Packed >> 24) & 0xFF)};
    
    return Result;
}

inline u32
RGBAPack4x8(v4 Unpacked) {
    u32 Result = ((RoundF32ToU32(Unpacked.a) << 24) |
                  (RoundF32ToU32(Unpacked.b) << 16) |
                  (RoundF32ToU32(Unpacked.g) << 8) |
                  (RoundF32ToU32(Unpacked.r) << 0));
    
    return Result;
}

inline v4
LinearRGBAToHSVA(v4 InColor) {
    f32 Min = Min(InColor.r, Min(InColor.b, InColor.g));
    f32 Max = Max(InColor.r, Max(InColor.b, InColor.g));
    
    f32 Delta = Max - Min;
    
    f32 V = Max;
    f32 S = 0;
    
    f32 Tolerance = 0.0001f;
    if(Max > Tolerance) //Tolerance for divide by zero
    {
        S = Delta / Max;
    }
    
    f32 H;
    if(Delta > Tolerance) {
        if(InColor.r == Max) {
            H = (InColor.g - InColor.b) / Delta;
        }
        else if(InColor.g == Max) {
            H = 2 + (InColor.b - InColor.r) / Delta;
        } else {
            H = 4 + (InColor.r - InColor.g) / Delta;
        }
    } else {
        H = 0;
    }
    if(H < 0){H += 6;}
    H /= 6;
    
    return(V4(H, S, V, InColor.a));
}

inline v4
LinearHSVAToRGBA(v4 InColor) {
    v4 Result = {};
    Result.a = InColor.a;
    
    s32 I = FloorF32ToS32(InColor.h * 6);
    f32 F = InColor.h * 6 - I;
    f32 P = InColor.v * (1 - InColor.s);
    f32 Q = InColor.v * (1 - F * InColor.s);
    f32 T = InColor.v * (1 - (1 - F) * InColor.s);
    
    switch(I%6) {
        case(0):{ Result.r = InColor.v, Result.g = T,         Result.b = P;} break;
        case(1):{ Result.r = Q,         Result.g = InColor.v, Result.b = P;} break;
        case(2):{ Result.r = P,         Result.g = InColor.v, Result.b = T;} break;
        case(3):{ Result.r = P,         Result.g = Q,         Result.b = InColor.v;} break;
        case(4):{ Result.r = T,         Result.g = P,         Result.b = InColor.v;} break;
        case(5):{ Result.r = InColor.v, Result.g = P,         Result.b = Q;} break;
    }
    return Result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Matrix 2x2 operations
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
inline m2x2
operator*(m2x2 A, m2x2 B) {
    m2x2 R = {};
    
    for(int r = 0; r < 2; ++r) { // NOTE(Jesse): Rows (of A) {
        for(int c = 0; c < 2; ++c) { // NOTE(Jesse): Column (of B) {
            for(int i = 0; i < 2; ++i)  // NOTE(Jesse): Columns of A, rows of B!
            {
                R.E[r][c] += A.E[r][i]*B.E[i][c];
            }
        }
    }
    return R;
}

inline v2
Transform(m2x2 A, v2 P) {
    v2 R;
    
    R.x = P.x*A.E[0][0] + P.y*A.E[0][1];
    R.y = P.x*A.E[1][0] + P.y*A.E[1][1];
    
    return R;
}

inline v2
operator*(m2x2 A, v2 P) {
    v2 R = Transform(A, P);
    return R;
}

inline m2x2
M2x2Identity(void) {
    m2x2 R = 
    {
        {   {1, 0},
            {0, 1}},
    };
    return R;
}

inline m2x2
M2x2Rotation(f32 Angle) {
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);
    m2x2 R =
    {
        {   {c, -s},
            {s,  c}},
    };
    
    return R;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Matrix 4x4 operations
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

inline m4x4
operator*(m4x4 A, m4x4 B) {
    // NOTE(Jesse): This is written to be instructive, not optimal!
    m4x4 R = {};
    
    for(int r = 0; r < 4; ++r) { // NOTE(Jesse): Rows (of A) {
        for(int c = 0; c < 4; ++c) { // NOTE(Jesse): Column (of B) {
            for(int i = 0; i < 4; ++i)  // NOTE(Jesse): Columns of A, rows of B!
            {
                R.E[r][c] += A.E[r][i]*B.E[i][c];
            }
        }
    }
    
    return R;
}

inline v4
Transform(m4x4 A, v4 P) {
    // NOTE(Jesse): This is written to be instructive, not optimal!
    v4 R;
    
    R.x = P.x*A.E[0][0] + P.y*A.E[0][1] + P.z*A.E[0][2] + P.w*A.E[0][3];
    R.y = P.x*A.E[1][0] + P.y*A.E[1][1] + P.z*A.E[1][2] + P.w*A.E[1][3];
    R.z = P.x*A.E[2][0] + P.y*A.E[2][1] + P.z*A.E[2][2] + P.w*A.E[2][3];
    R.w = P.x*A.E[3][0] + P.y*A.E[3][1] + P.z*A.E[3][2] + P.w*A.E[3][3];
    
    return R;
}

inline v2
operator*(m4x4 A, v2 P) {
    v2 R = Transform(A, V4(P.x, P.y, 1.0, 1.0)).xy;
    return R;
}

inline v3
operator*(m4x4 A, v3 P) {
    v3 R = Transform(A, V4(P, 1.0f)).xyz;
    return R;
}

inline v4
operator*(m4x4 A, v4 P) {
    v4 R = Transform(A, P);
    return R;
}

inline m4x4
M4x4Identity(void) {
    m4x4 R =
    {
        {{1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}},
    };
    
    return R;
}

inline m4x4
XRotation(f32 Angle) {
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);
    
    m4x4 R =
    {
        {   {1, 0, 0, 0},
            {0, c,-s, 0},
            {0, s, c, 0},
            {0, 0, 0, 1}},
    };
    
    return R;
}

inline m4x4
YRotation(f32 Angle) {
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);
    
    m4x4 R =
    {
        {   { c, 0, s, 0},
            { 0, 1, 0, 0},
            {-s, 0, c, 0},
            { 0, 0, 0, 1}},
    };
    
    return R;
}

inline m4x4
ZRotation(f32 Angle) {
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);
    
    m4x4 R =
    {
        {   {c,-s, 0, 0},
            {s, c, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}},
    };
    
    return R;
}

inline m4x4
Translation(v3 T)
{
    m4x4 R =
    {
        {{1, 0, 0, T.x},
            {0, 1, 0, T.y},
            {0, 0, 1, T.z},
            {0, 0, 0, 1}},
    };
    
    return R;
}

inline m4x4
Transpose(m4x4 A) {
    m4x4 R;
    
    for(int j = 0; j <= 3; ++j) {
        for(int i = 0; i <= 3; ++i) {
            R.E[j][i] = A.E[i][j];
        }
    }
    
    return R;
}

inline m4x4
Columns3x3(v3 X, v3 Y, v3 Z) {
    m4x4 R =
    {
        {{X.x, Y.x, Z.x, 0},
            {X.y, Y.y, Z.y, 0},
            {X.z, Y.z, Z.z, 0},
            {  0,   0,   0, 1}}
    };
    
    return R;
}

inline m4x4
Rows3x3(v3 X, v3 Y, v3 Z) {
    m4x4 R =
    {
        {{X.x, X.y, X.z, 0},
            {Y.x, Y.y, Y.z, 0},
            {Z.x, Z.y, Z.z, 0},
            {  0,   0,   0, 1}}
    };
    
    return R;
}

inline m4x4
Translate(m4x4 A, v3 T) {
    m4x4 R = A;
    
    R.E[0][3] += T.x;
    R.E[1][3] += T.y;
    R.E[2][3] += T.z;
    
    return R;
}

inline m4x4
Scale(m4x4 A, v3 S) {
    m4x4 R = A;
    
    R.E[0][0] *= S.x;
    R.E[1][1] *= S.y;
    R.E[2][2] *= S.z;
    
    return R;
}

inline v3
GetColumn(m4x4 A, u32 C) {
    v3 R = {A.E[0][C], A.E[1][C], A.E[2][C]};
    return R;
}

inline v3
GetRow(m4x4 A, u32 R) {
    v3 Result = {A.E[R][0], A.E[R][1], A.E[R][2]};
    return Result;
}

inline m4x4_inv
CameraTransform(v3 X, v3 Y, v3 Z, v3 P) {
    m4x4_inv Result;
    m4x4 A = Rows3x3(X, Y, Z);
    v3 AP = -(A*P);
    A = Translate(A, AP);
    Result.Forward = A;
    
    v3 iX = X/LengthSq(X);
    v3 iY = Y/LengthSq(Y);
    v3 iZ = Z/LengthSq(Z);
    v3 iP = 
    {   AP.x*iX.x + AP.y*iY.x + AP.z*iZ.x,
        AP.x*iX.y + AP.y*iY.y + AP.z*iZ.y,
        AP.x*iX.z + AP.y*iY.z + AP.z*iZ.z  };
    
    m4x4 B = Columns3x3(iX, iY, iZ);
    B = Translate(B, -iP);
    Result.Inverse = B;
    
    return Result;
}

inline m4x4_inv
LookAt(v3 From, v3 To, v3 Up = V3(0, 1, 0)) {
    v3 ZAxis = Normalize(From - To);
    v3 XAxis = Normalize(Cross(Up, ZAxis));
    v3 YAxis = Cross(ZAxis, XAxis);
    
    return CameraTransform(XAxis, YAxis, ZAxis, From);
}

inline m4x4_inv
PerspectiveProjection(f32 AspectRatio, f32 FocalLength, f32 NearClipPlane, f32 FarClipPlane)
{
    f32 n = NearClipPlane; // NOTE(Jesse): Near clip plane _distance_
    f32 f = FarClipPlane; // NOTE(Jesse): Far clip plane _distance_
    
    f32 a = (f32)tan(FocalLength / 2);
    f32 b = AspectRatio;
    f32 c = -(f + n)/(f - n);
    f32 d = -(2*f*n) / (f - n);
    
    m4x4_inv Result;
    Result.Forward = 
    {
        {   {1/(b*a), 0,   0,  0},
            {0,   1/a, 0,  0},
            {0,   0,   c,  d},
            {0,   0,  -1,  0},
        }
    };
    
    Result.Inverse = 
    {
        {   {1/(1/(b*a)), 0,   0,  0},
            {0,     1/(1/a),   0,  0},
            {0,           0,   0, -1},
            {0,           0, 1/d,  c/d},
        }
    };
    
    return Result;
}


inline m4x4_inv
OrthographicProjection(v2 BoundingX, v2 BoundingY, v2 BoundingZ) {
    f32 l = BoundingX.x;
    f32 r = BoundingX.y;
    
    f32 t = BoundingY.y;
    f32 b = BoundingY.x;
    
    f32 n = BoundingZ.x;
    f32 f = BoundingZ.y;
    
    f32 A  = 2.0f/(r-l);
    f32 B  = 2.0f/(t-b);
    f32 C  = -2.0f/(f-n);
    
    f32 X = -(r+l)/(r-l);
    f32 Y = -(t+b)/(t-b);
    f32 Z = -(f+n)/(f-n);
    
    m4x4_inv Result;
    Result.Forward = 
    {
        {   {A,  0,  0,  X},
            {0,  B,  0,  Y},
            {0,  0,  C,  Z},
            {0,  0,  0,  1},
        }
    };
    
    Result.Inverse =
    {
        {   {1/A,  0,   0,   -X/A},
            {0,    1/B, 0,   -Y/B},
            {0,    0,   1/C, -Z/C},
            {0,    0,   0,   1},
        }
    };
    
    return Result;
}

inline m4x4_inv
OrthographicProjection(f32 AspectWidthOverHeight, f32 NearClipPlane, f32 FarClipPlane)
{
    f32 a = 1.0f;
    f32 b = AspectWidthOverHeight;
    
    f32 n = NearClipPlane; // NOTE(jesse): Near clip plane _distance_
    f32 f = FarClipPlane; // NOTE(jesse): Far clip plane _distance_
    
    // NOTE(jesse): These are the non-perspective corrected terms, for orthographic
    f32 d = 2.0f / (n - f);
    f32 e = (n + f) / (n - f);
    
    m4x4_inv Result =
    {
        {{{a,  0,  0,  0},
                {0,  b,  0,  0},
                {0,  0,  d,  e},
                {0,  0,  0,  1}}},
        
        {{{1/a,   0,   0,    0},
                {  0, 1/b,   0,    0},
                {  0,   0, 1/d, -e/d},
                {  0,   0,   0,    1}}},
    };
    
    return Result;
}
//
// Quaternion operations
//

inline quaternion
operator*(quaternion Q1, quaternion Q2) {
    quaternion Result;
    Result.i =  Q1.i * Q2.r + Q1.j * Q2.k - Q1.k * Q2.j + Q1.r * Q2.i;
    Result.j = -Q1.i * Q2.k + Q1.j * Q2.r + Q1.k * Q2.i + Q1.r * Q2.j;
    Result.k =  Q1.i * Q2.j - Q1.j * Q2.i + Q1.k * Q2.r + Q1.r * Q2.k;
    Result.r = -Q1.i * Q2.i - Q1.j * Q2.j - Q1.k * Q2.k + Q1.r * Q2.r;
    
    return Result;
}

inline quaternion // NOTE(Jesse):THIS USES RADIANS
RotToQuat(f32 Angle, v3 Axis) {
    quaternion Result;
    Result.i = Sin(Angle/2)*Axis.x;
    Result.j = Sin(Angle/2)*Axis.y;
    Result.k = Sin(Angle/2)*Axis.z;
    Result.r = Cos(Angle/2);
    
    return Result;
}

inline quaternion
Inverse(quaternion Q) {
    quaternion Result;
    Result.i = -Q.i;
    Result.j = -Q.j;
    Result.k = -Q.k;
    Result.r = Q.r;
    return Result;
}

inline m4x4
QuatToRotM4x4(quaternion Q) {
    f32 iSq = Q.i*Q.i;
    f32 jSq = Q.j*Q.j;
    f32 kSq = Q.k*Q.k;
    m4x4 Result = 
    {
        {   {(1 - 2*(jSq + kSq)), (2*(Q.i*Q.j - Q.r*Q.k)), (2*(Q.i*Q.k + Q.r*Q.j)), 0},
            {(2*(Q.i*Q.j + Q.r*Q.k)), (1-2*(iSq + kSq)), (2*(Q.j*Q.k - Q.r*Q.i)), 0},
            {(2*(Q.i*Q.k - Q.r*Q.j)), (2*(Q.j*Q.k + Q.r*Q.i)), (1-2*(iSq + jSq)), 0},
            {0, 0, 0, 1}}
    };
    
    return Result;
}


inline quaternion
Normalize(quaternion In) {
    quaternion Q = In;
    f32 n = (f32)sqrt(Q.i*Q.i + Q.j*Q.j + Q.k*Q.k + Q.r*Q.r);
    Q.i /= n;
    Q.j /= n;
    Q.k /= n;
    Q.r /= n;
    
    return(Q);
}

//
// Ray Tracing Operaions
//


inline v2
RayIntersect2(v2 Pa, v2 ra, v2 Pb, v2 rb) {
    v2 Result = {};
    
    /* NOTE(Jesse):
    
       Pa.x + ta*ra.x = Pb.x + tb*rb.x
       Pa.y + ta*ra.y = Pb.y + tb*rb.y
    */
    
    f32 d = (rb.x*ra.y - rb.y*ra.x);
    if(d != 0.0f) {
        f32 ta = ((Pa.x - Pb.x)*rb.y + (Pb.y - Pa.y)*rb.x) / d;
        f32 tb = ((Pa.x - Pb.x)*ra.y + (Pb.y - Pa.y)*ra.x) / d;
        
        Result = V2(ta, tb);
    }
    
    return Result;
}


inline f32
RayWithSphere(v3 RayOrigin, v3 RayDirection, sphere Sphere) {
    f32 Result = 0;
    
    v3 SphereRelativeRayOrigin = RayOrigin - Sphere.P;
    f32 a = Inner(RayDirection, RayDirection);
    f32 b = 2.0f*Inner(RayDirection, SphereRelativeRayOrigin);
    f32 c = Inner(SphereRelativeRayOrigin, SphereRelativeRayOrigin) - (Sphere.r*Sphere.r);
    
    f32 Tolerance = 0.0001f;
    f32 MinHitDistance = 0.001f;
    f32 MaxHitDistance = 100.0f;
    
    f32 Denom = 2.0f*a;
    f32 RootTerm  = SquareRoot(b*b - 4.0f*a*c);
    
    if(RootTerm > Tolerance) {
        f32 tp = (-b + RootTerm) / Denom;
        f32 tn = (-b - RootTerm) / Denom;
        
        f32 t = tp;
        if((tn > MinHitDistance) && (tn < tp)) {
            t = tn;
        }
        if((t > MinHitDistance) && (t < MaxHitDistance)) {
            Result = t;
        }
    }
    
    return Result;
}

inline f32
RayWithPlane(v3 RayOrigin, v3 RayDirection, plane Plane) {
    f32 Result = 0;
    
    f32 Tolerance = 0.0001f;
    f32 MinHitDistance = 0.001f;
    f32 MaxHitDistance = 100.0f;
    f32 Denom = Inner(Plane.N, RayDirection);
    if(AbsoluteValue(Denom) > Tolerance) {
        f32 t = -(Plane.d + Inner(Plane.N, RayOrigin)) / Denom;
        if((t > MinHitDistance) && (t < MaxHitDistance)) {
            Result = t;
        }
    }
    return Result;
}


inline f32
RayWithDisk(v3 RayOrigin, v3 RayDirection, disk Disk, f32* DistanceCenter) {
    plane Plane;
    Plane.N = Disk.N;
    Plane.d = Disk.d;
    
    f32 Result = RayWithPlane(RayOrigin, RayDirection, Plane);
    
    if(Result) {
        v3 P = RayOrigin + RayDirection * Result;
        v3 V = P - (Disk.N * Disk.d);
        f32 DistanceToCenter = Inner(V, V);
        if(sqrt(DistanceToCenter) > Disk.r) {
            Result = 0;
        } else {
            *DistanceCenter = DistanceToCenter;
        }
    }
    return Result;
}

inline f32
RayWithBox(v3 RayOrigin, v3 RayDirection, rectangle3 Box) {
    f32 Result = FLT_MAX;
    
    f32 xMin = Box.Min.x;
    f32 xMax = Box.Max.x;
    f32 yMin = Box.Min.y;
    f32 yMax = Box.Max.y;
    f32 zMin = Box.Min.z;
    f32 zMax = Box.Max.z;
    
    // Inverse distance calculation
    f32 t_xMin = (xMin - RayOrigin.x) / RayDirection.x;
    f32 t_xMax = (xMax - RayOrigin.x) / RayDirection.x;
    f32 t_yMin = (yMin - RayOrigin.y) / RayDirection.y;
    f32 t_yMax = (yMax - RayOrigin.y) / RayDirection.y;
    f32 t_zMin = (zMin - RayOrigin.z) / RayDirection.z;
    f32 t_zMax = (zMax - RayOrigin.z) / RayDirection.z;
    
    // Probable x,y,z hit locations
    f32 x;
    f32 y;
    f32 z;
    
    if(t_xMin > 0) {
        y = RayOrigin.y + (RayDirection.y * t_xMin);
        z = RayOrigin.z + (RayDirection.z * t_xMin);
        
        if((y <= yMax) && (y >= yMin) && (z <= zMax) && (z >= zMin)) {
            if(t_xMin < Result) {
                Result = t_xMin;
            }
        }
    }
    
    if(t_xMax > 0) {
        y = RayOrigin.y + (RayDirection.y * t_xMax);
        z = RayOrigin.z + (RayDirection.z * t_xMax);
        
        if((y <= yMax) && (y >= yMin) && (z <= zMax) && (z >= zMin)) {
            if(t_xMax < Result) {
                Result = t_xMax;
            }
        }
    }
    
    if(t_yMin > 0) {
        x = RayOrigin.x + (RayDirection.x * t_yMin);
        z = RayOrigin.z + (RayDirection.z * t_yMin);
        
        if((x <= xMax) && (x >= xMin) && (z <= zMax) && (z >= zMin)) {
            if(t_yMin < Result) {
                Result = t_yMin;
            }
        }
    }
    
    if(t_yMax > 0) {
        x = RayOrigin.x + (RayDirection.x * t_yMax);
        z = RayOrigin.z + (RayDirection.z * t_yMax);
        
        if((x <= xMax) && (x >= xMin) && (z <= zMax) && (z >= zMin)) {
            if(t_yMax < Result) {
                Result = t_yMax;
            }
        }
    }
    
    if(t_zMin > 0) {
        y = RayOrigin.y + (RayDirection.y * t_zMin);
        x = RayOrigin.x + (RayDirection.x * t_zMin);
        
        if((x <= xMax) && (x >= xMin) && (y <= yMax) && (y >= yMin)) {
            if(t_zMin < Result) {
                Result = t_zMin;
            }
        }
    }
    
    if(t_zMax > 0) {
        y = RayOrigin.y + (RayDirection.y * t_zMax);
        x = RayOrigin.x + (RayDirection.x * t_zMax);
        
        if((x <= xMax) && (x >= xMin) && (y <= yMax) && (y >= yMin)) {
            if(t_zMax < Result) {
                Result = t_zMax;
            }
        }
    }
    
    if(Result < 0.1f || Result > 100.0f) {
        Result = 0;
    }
    
    return Result;
}
