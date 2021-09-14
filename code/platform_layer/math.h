/* date = July 30th 2020 1:05 pm */

#ifndef MATH_H
#define MATH_H

#include "defines.h"
union v2
{
    struct {
        f32 x, y;
    };
    struct {
        f32 u, v;
    };
    struct {
        f32 Width, Height;
    };
    f32 E[2];
};

union v2u {
    
    struct {
        u32 x, y;
    };
    struct {
        u32 Width, Height;
    };
    u32 E[2];
};

union v2s {
    struct {
        s32 x, y;
    };
    struct {
        s32 Width, Height;
    };
    s32 E[2];
};

union v3 {
    struct {
        f32 x, y, z;
    };
    struct {
        f32 u, v, w;
    };
    struct {
        f32 r, g, b;
    };
    struct {
        v2 xy;
        f32 Ignored0_;
    };
    struct {
        f32 Ignored1_;
        v2 yz;
    };
    struct {
        v2 uv;
        f32 Ignored2_;
    };
    struct {
        f32 Ignored3_;
        v2 vw;
    };
    f32 E[3];
};

union v4 {
    struct {
        union {
            v3 xyz;
            struct {
                f32 x, y, z;
            };
        };
        f32 w;        
    };
    struct {
        union {
            v3 rgb;
            struct {
                f32 r, g, b;
            };
        };
        
        f32 a;        
    };
    
    struct {
        union {
            v3 hsv;
            struct {
                f32 h, s, v;
            };
        };
        
        f32 a;        
    };
    struct {
        v2 xy;
        f32 Ignored0_;
        f32 Ignored1_;
    };
    struct {
        f32 Ignored2_;
        v2 yz;
        f32 Ignored3_;
    };
    struct {
        f32 Ignored4_;
        f32 Ignored5_;
        v2 zw;
    };
    f32 E[4];
};

inline v2 V2(f32 A, f32 B) {
    v2 Result = {A, B};
    return(Result);
}

inline v2 V2(f32 A) {
    return V2(A, A);
}

inline v2 V2(v2u A) {
    v2 Result = {(f32)A.x, (f32)A.y};
    return(Result);
}

inline v2 V2(v2s A) {
    v2 Result = {(f32)A.x, (f32)A.y};
    return(Result);
}

inline v2u V2u(u32 A, u32 B) {
    v2u Result = {A, B};
    return(Result);
}

inline v2s V2s(s32 A, s32 B) {
    v2s Result = {A, B};
    return(Result);
}

inline v3 V3(f32 A, f32 B, f32 C) {
    v3 Result = {A, B, C};
    return(Result);
}

inline v4 V4(f32 A, f32 B, f32 C, f32 D) {
    v4 Result = {A, B, C, D};
    return(Result);
}


inline v3 V3i(s32 A, s32 B, s32 C) {
    v3 Result = {(f32)A, (f32)B, (f32)C};
    return(Result);
}

inline v4 Vi(s32 A, s32 B, s32 C, s32 D) {
    v4 Result = {(f32)A, (f32)B, (f32)C, (f32)D};
    return(Result);
}

inline v3 V3(f32 A) {return(V3(A, A, A));};
inline v3 V3(v2 A, f32 B) {return(V3(A.x, A.y, B));};
inline v3 V3(f32 A, v2 B) {return(V3(A, B.x, B.y));};

inline v4 V4(f32 A) {return(V4(A, A, A, A));};
inline v4 V4(v2 A, v2 B) {return(V4(A.x, A.y, B.x, B.y));};
inline v4 V4(v3 A, f32 B) {return(V4(A.x, A.y, A.z, B));};
inline v4 V4(f32 A, v3 B) {return(V4(A, B.x, B.y, B.z));};


union quaternion {
    struct {
        union {
            v3 ijk;
            struct {
                f32 i, j, k;
            };
        };
        f32 r;        
    };
    struct {
        v2 ij;
        f32 Ignored0_;
        f32 Ignored1_;
    };
    struct {
        f32 Ignored2_;
        v2 jk;
        f32 Ignored3_;
    };
    struct {
        f32 Ignored4_;
        f32 Ignored5_;
        v2 kr;
    };
    f32 E[4];
};


inline quaternion Q(f32 A, f32 B, f32 C, f32 D) {
    quaternion Result = {A, B, C, D};
    return(Result);
}

inline quaternion Q(v2 A, v2 B) {return(Q(A.x, A.y, B.x, B.y));};
inline quaternion Q(v3 A, f32 B) {return(Q(A.x, A.y, A.z, B));};
inline quaternion Q(f32 A, v3 B) {return(Q(A, B.x, B.y, B.z));};

struct m2x2 {
    f32 E[4][4];
};

struct m4x4 {
    f32 E[4][4];
};

struct m4x4_inv {
    m4x4 Forward;
    m4x4 Inverse;
};

struct triangle2 {
    v2 P0;
    v2 P1;
    v2 P2;
};

struct quad2 {
    v2 P0;
    v2 P1;
    v2 P2;
    v2 P3;
};

union rectangle2 {
    struct {
        v2 Min;
        v2 Max;
    };
    
    struct {
        f32 x0;
        f32 y0;
        f32 x1;
        f32 y1;
    };
};


union rectangle3 {
    struct {
        v3 Min;
        v3 Max;
    };
    
    struct {
        f32 x0;
        f32 y0;
        f32 z0;
        f32 x1;
        f32 y1;
        f32 z1;
    };
};

struct plane {
    v3 N;
    f32 d;
};

struct circle {
    v2 P;
    f32 r;
};

struct sphere {
    v3 P;
    f32 r;
};

struct disk {
    v3 N;
    f32 d;
    f32 r;
};

#endif //MATH_H
