/* date = September 24th 2021 8:29 pm */

#ifndef FONT_LOADING_H
#define FONT_LOADING_H

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#endif

#define FONT_STARTING_CODE_POINT 32
#define FONT_CODE_POINT_COUNT 256

unsigned char font_ttf_buffer[1<<20];
static asset_font_atlas
LoadFontAtlasFromFile(char* FileName, s32 FontSize) {
    asset_font_atlas Result = {};
    s32 UnicodeStart = FONT_STARTING_CODE_POINT;
    s32 CharCount = FONT_CODE_POINT_COUNT;
    
    fread(font_ttf_buffer, 1, 1<<20, fopen(FileName, "rb"));
    stbtt_fontinfo Font;
    
    stbtt_InitFont(&Font, font_ttf_buffer, 0);
    
    stbtt_packedchar* stb_Char_Infos = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar) * (CharCount + 1));
    s32 TextureSize = 128;
    u8* AlphaBitmap = NULL;
    while(1) {
        AlphaBitmap = (u8*)malloc(TextureSize * TextureSize * sizeof(u8) * 2);
        
        stbtt_pack_context PackContext;
        stbtt_PackBegin(&PackContext, AlphaBitmap, TextureSize, TextureSize, 0, 1, 0);
        //stbtt_PackSetOversampling(&PackContext, 2, 2);
        
        if(!stbtt_PackFontRange(&PackContext, font_ttf_buffer, 0, FontSize, UnicodeStart, CharCount, stb_Char_Infos)) {
            free(AlphaBitmap);
            stbtt_PackEnd(&PackContext);
            TextureSize *= 2;
        } else {
            stbtt_PackEnd(&PackContext);
            break;
        }
    }
    
    asset_bitmap Bitmap = {};
    Bitmap.Width = TextureSize;
    Bitmap.Height = TextureSize;
    Bitmap.Channels = 4;
    Bitmap.Data = (u8*)malloc(Bitmap.Channels*Bitmap.Width*Bitmap.Height);
    
    if(Bitmap.Data) {
        u8 *Source = AlphaBitmap;
        u32 *DestRow = (u32 *)Bitmap.Data + (Bitmap.Height - 1)*Bitmap.Width;
        for(u32 Y = 0;
            Y < Bitmap.Height;
            ++Y)
        {
            u32* Dest = DestRow;
            for(u32 X = 0;
                X < Bitmap.Width;
                ++X)
            {
                u8 Pixel = *Source++;
                f32 Gray = (f32)(Pixel & 0xff);
                v4 Texel = V4(255.0f, 255.0f, 255.0f, Gray);
                Texel = SRGB255ToLinear1(Texel);
                Texel.rgb *= Texel.a;
                Texel = Linear1ToSRGB255(Texel);
                
                *Dest++ = (((u32)(Texel.a + 0.5f) << 24) |
                           ((u32)(Texel.r + 0.5f) << 16) |
                           ((u32)(Texel.g + 0.5f) << 8) |
                           ((u32)(Texel.b + 0.5f) << 0));
            }
            
            DestRow -= Bitmap.Width;
        }
    } else {
        Assert(!"Unable To Allocate Bitmap Data");
    }
    
    Result.Bitmap = Bitmap;
    
    Result.CharInfos = (font_char_info*)malloc(sizeof(font_char_info)* (CharCount));
    
    //NOTE(Jesse): Convering Character Info To Bottom Up and 0-1 UV Space
    f32 InvTextureWidth = 1.0f/(f32)Bitmap.Width;
    f32 InvTextureHeight = 1.0f/(f32)Bitmap.Height;
    for(s32 CharIndex = 0;
        CharIndex < CharCount;
        CharIndex++) {
        
        stbtt_packedchar* Source = &stb_Char_Infos[CharIndex];
        font_char_info* Dest = &Result.CharInfos[CharIndex];
        
        Dest->XAdvance = Source->xadvance;
        Dest->XOffsetMin = Source->xoff;
        Dest->XOffsetMax = Source->xoff2;
        Dest->YOffsetMin = -Source->yoff2;
        Dest->YOffsetMax = -Source->yoff;
        Dest->UVMin = V2((f32)Source->x0 * InvTextureWidth, ((f32)(Bitmap.Height - Source->y1) * InvTextureHeight));
        Dest->UVMax = V2((f32)Source->x1 * InvTextureWidth, ((f32)(Bitmap.Height - Source->y0) * InvTextureHeight));
    }
    
    
    Result.Scale = stbtt_ScaleForPixelHeight(&Font, FontSize);
    stbtt_GetFontVMetrics(&Font, &Result.Ascent, 0, 0);
    Result.Baseline = (s32)(Result.Ascent * Result.Scale);
    Result.OriginalSize = FontSize;
    Result.UnicodeStart = UnicodeStart;
    
    free(AlphaBitmap);
    free(stb_Char_Infos);
    return Result;
}

#endif //FONT_LOADING_H
