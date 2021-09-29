/* date = August 18th 2021 7:52 pm */

#ifndef ASSET_H
#define ASSET_H

enum asset_type {
    ASSET_TYPE_NONE,
    ASSET_TYPE_BITMAP,
    ASSET_TYPE_ATLAS,
    ASSET_TYPE_FONT_ATLAS,
    ASSET_TYPE_CUBEMAP,
    ASSET_TYPE_MESH,
    ASSET_TYPE_MESH_ARRAY,
    ASSET_TYPE_SOUND_S16,
};

struct asset_bitmap {
    u32 Width;
    u32 Height;
    u32 Channels;
    u8* Data;
};

struct atlas_bitmap_info {
    v2 UVMin;
    v2 UVMax;
};

struct asset_atlas {
    u32 Width;
    u32 Height;
    u32 Channels;
    u8* Data;
    atlas_bitmap_info* Infos;
};

struct font_char_info {
    f32 XAdvance;
    f32 XOffsetMin, XOffsetMax;
    f32 YOffsetMin, YOffsetMax;
    v2 UVMin;
    v2 UVMax;
};

struct asset_font_atlas {
    asset_bitmap Bitmap;
    s32 OriginalSize;
    f32 Scale;
    s32 Ascent;
    s32 Baseline;
    s32 Advance;
    s32 LeftSideBearing;
    s32 UnicodeStart;
    font_char_info* CharInfos;
};

struct asset_cubemap {
    u32 Width;
    u32 Height;
    u32 Channels;
    u8* Data[6];
};

struct asset_mesh {
    v3* P;
    v2* UV;
    v3* N;
    v3* Tangents;
    v3* BiTangents;
    u32 VertexCount;
    
    u32* Indices;
    u32 IndexCount;
};

struct asset_mesh_array {
    u32 MeshCount;
    asset_mesh* Meshes;
};

struct asset_sound {
    u32 SampleCount;
    u32 ChannelCount;
    s16* Samples;
};

struct asset {
    u32 Type;
    union {
        asset_bitmap Bitmap;
        asset_atlas Atlas;
        asset_font_atlas Font;
        asset_cubemap Cubemap;
        asset_mesh Mesh;
        asset_mesh_array MeshArray;
        asset_sound Sound;
    };
};

#endif //ASSET_H
