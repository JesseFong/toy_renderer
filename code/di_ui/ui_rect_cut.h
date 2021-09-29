/* date = March 26th 2021 3:12 pm */

#ifndef UI_RECT_CUT_H
#define UI_RECT_CUT_H

enum ui_cut_side {
    UI_CUT_LEFT,
    UI_CUT_RIGHT,
    UI_CUT_TOP,
    UI_CUT_BOTTOM,
};

struct rect_cut_result {
    rectangle2* Rect;
    rectangle2* PrevRect;
};

static rectangle2
UIGetLeft(rectangle2* Rect, f32 A) {
    rectangle2 Result = {};
    f32 MinX = Rect->Min.x;
    f32 MaxX = Min(Rect->Max.x, Rect->Min.x + A);
    Result = RectMinMax(MinX, Rect->Min.y, MaxX, Rect->Max.y);
    return Result;
}

static rectangle2
UIGetRight(rectangle2* Rect, f32 A) {
    rectangle2 Result = {};
    f32 MaxX = Rect->Max.x;
    f32 MinX = Max(Rect->Min.x, Rect->Max.x - A);
    Result = RectMinMax(MinX, Rect->Min.y, MaxX, Rect->Max.y);
    return Result;
}

static rectangle2
UIGetTop(rectangle2* Rect, f32 A) {
    rectangle2 Result = {};
    f32 MaxY = Rect->Max.y;
    f32 MinY = Max(Rect->Min.y, Rect->Max.y - A);
    Result = RectMinMax(Rect->Min.x, MinY, Rect->Max.x, MaxY);
    return Result;
}

static rectangle2
UIGetBottom(rectangle2* Rect, f32 A) {
    rectangle2 Result = {};
    f32 MinY = Rect->Min.y;
    f32 MaxY = Min(Rect->Max.y, Rect->Min.y + A);
    Result = RectMinMax(Rect->Min.x, MinY, Rect->Max.x, MaxY);
    return Result;
}

static rectangle2
UICutLeft(rectangle2* Rect, f32 A) {
    rectangle2 Result = UIGetLeft(Rect, A);
    Rect->Min.x = Result.Max.x;
    return Result;
}

static rectangle2
UICutRight(rectangle2* Rect, f32 A) {
    rectangle2 Result = UIGetRight(Rect, A);
    Rect->Max.x = Result.Min.x;
    return Result;
}

static rectangle2
UICutTop(rectangle2* Rect, f32 A) {
    rectangle2 Result = UIGetTop(Rect, A);
    Rect->Max.y = Result.Min.y;
    return Result;
}

static rectangle2
UICutBottom(rectangle2* Rect, f32 A) {
    rectangle2 Result = UIGetBottom(Rect, A);
    Rect->Min.y = Result.Max.y;
    return Result;
}

static rectangle2
UICutSide(rectangle2* Rect, f32 A, u32 CutSide) {
    rectangle2 Result = {};
    switch(CutSide) {
        case UI_CUT_LEFT: {
            Result = UICutLeft(Rect, A);
        } break;
        
        case UI_CUT_RIGHT: {
            Result = UICutRight(Rect, A);
        } break;
        
        case UI_CUT_TOP: {
            Result = UICutTop(Rect, A);
        } break;
        
        case UI_CUT_BOTTOM: {
            Result = UICutBottom(Rect, A);
        } break;
    }
    return Result;
}

static rectangle2
UIGetSide(rectangle2* Rect, f32 A, u32 CutSide) {
    rectangle2 Result = {};
    switch(CutSide) {
        case UI_CUT_LEFT: {
            Result = UIGetLeft(Rect, A);
        } break;
        
        case UI_CUT_RIGHT: {
            Result = UIGetRight(Rect, A);
        } break;
        
        case UI_CUT_TOP: {
            Result = UIGetTop(Rect, A);
        } break;
        
        case UI_CUT_BOTTOM: {
            Result = UIGetBottom(Rect, A);
        } break;
    }
    return Result;
}

static rectangle2
UICutSideRatio(rectangle2* Rect, f32 Ratio, u32 CutSide) {
    Assert(Ratio <= 1.0f);
    rectangle2 Result = {};
    f32 Width = GetWidth(*Rect);
    f32 Height = GetHeight(*Rect);
    
    switch(CutSide) {
        case UI_CUT_LEFT: {
            Result = UICutLeft(Rect, Ratio*Width);
        } break;
        case UI_CUT_RIGHT: {
            Result = UICutRight(Rect, Ratio*Width);
        } break;
        case UI_CUT_TOP: {
            Result = UICutTop(Rect, Ratio*Height);
        } break;
        case UI_CUT_BOTTOM: {
            Result = UICutBottom(Rect, Ratio*Height);
        } break;
    }
    return Result;
}

static rectangle2
UICutLeftRatio(rectangle2* Rect, f32 Ratio) {
    return UICutSideRatio(Rect, Ratio, UI_CUT_LEFT);
}

static rectangle2
UICutRightRatio(rectangle2* Rect, f32 Ratio) {
    return UICutSideRatio(Rect, Ratio, UI_CUT_RIGHT);
}

static rectangle2
UICutTopRatio(rectangle2* Rect, f32 Ratio) {
    return UICutSideRatio(Rect, Ratio, UI_CUT_TOP);
}

static rectangle2
UICutBottomRatio(rectangle2* Rect, f32 Ratio) {
    return UICutSideRatio(Rect, Ratio, UI_CUT_BOTTOM);
}

static rectangle2
UIGetSideRatio(rectangle2* Rect, f32 Ratio, u32 CutSide) {
    Assert(Ratio <= 1.0f);
    rectangle2 Result = {};
    f32 Width = GetWidth(*Rect);
    f32 Height = GetHeight(*Rect);
    
    switch(CutSide) {
        case UI_CUT_LEFT: {
            Result = UIGetLeft(Rect, Ratio*Width);
        } break;
        case UI_CUT_RIGHT: {
            Result = UIGetRight(Rect, Ratio*Width);
        } break;
        case UI_CUT_TOP: {
            Result = UIGetTop(Rect, Ratio*Height);
        } break;
        case UI_CUT_BOTTOM: {
            Result = UIGetBottom(Rect, Ratio*Height);
        } break;
    }
    return Result;
}

static rectangle2
UIGetLeftRatio(rectangle2* Rect, f32 Ratio) {
    return UIGetSideRatio(Rect, Ratio, UI_CUT_LEFT);
}

static rectangle2
UIGetRightRatio(rectangle2* Rect, f32 Ratio) {
    return UIGetSideRatio(Rect, Ratio, UI_CUT_RIGHT);
}

static rectangle2
UIGetTopRatio(rectangle2* Rect, f32 Ratio) {
    return UIGetSideRatio(Rect, Ratio, UI_CUT_TOP);
}

static rectangle2
UIGetBottomRatio(rectangle2* Rect, f32 Ratio) {
    return UIGetSideRatio(Rect, Ratio, UI_CUT_BOTTOM);
}


static f32
UIGetWidthFromRatio(rectangle2* Rect, f32 Ratio) {
    f32 Result = GetWidth(UIGetSideRatio(Rect, Ratio, UI_CUT_LEFT));
    return Result;
}

static f32
UIGetHeightFromRatio(rectangle2* Rect, f32 Ratio) {
    f32 Result = GetHeight(UIGetSideRatio(Rect, Ratio, UI_CUT_TOP));
    return Result;
}

static v2
UIGetSizeFromRatio(rectangle2* Rect, f32 Ratio) {
    v2 Result = {};
    Result.x = UIGetWidthFromRatio(Rect, Ratio);
    Result.y = UIGetHeightFromRatio(Rect, Ratio);
}

static rectangle2
UICutSubRect(rectangle2* Rect, f32 Top, f32 Bottom, f32 Left, f32 Right) {
    UICutTop(Rect, Top);
    UICutBottom(Rect, Bottom);
    UICutLeft(Rect, Left);
    UICutRight(Rect, Right);
    return *Rect;
}

static rectangle2
UICutSubRect(rectangle2* Rect, f32 Height, f32 Width) {
    rectangle2 Result = UICutSubRect(Rect, Height, Height, Width, Width);
    return Result;
}

static rectangle2
UIGetSubRect(rectangle2* Rect, f32 Top, f32 Bottom, f32 Left, f32 Right) {
    rectangle2 Result = *Rect;
    UICutSubRect(&Result, Top, Bottom, Left, Right);
    return Result;
}

static rectangle2
UIGetSubRect(rectangle2* Rect, f32 Height, f32 Width) {
    rectangle2 Result = UIGetSubRect(Rect, Height, Height, Width, Width);
    return Result;
}

static rectangle2
UICutSubRectRatio(rectangle2* Rect, f32 TopRatio, f32 BottomRatio, f32 LeftRatio, f32 RightRatio) {
    f32 Top = UIGetHeightFromRatio(Rect, TopRatio);
    f32 Bottom = UIGetHeightFromRatio(Rect, BottomRatio);
    f32 Left = UIGetWidthFromRatio(Rect, LeftRatio);
    f32 Right = UIGetWidthFromRatio(Rect, RightRatio);
    return UICutSubRect(Rect, Top, Bottom, Left, Right);
}

static rectangle2
UICutSubRectRatio(rectangle2* Rect, f32 Height, f32 Width) {
    rectangle2 Result = UICutSubRectRatio(Rect, Height, Height, Width, Width);
    return Result;
} 

static rectangle2
UIGetSubRectRatio(rectangle2* Rect, f32 TopRatio, f32 BottomRatio, f32 LeftRatio, f32 RightRatio) {
    rectangle2 Result = *Rect;
    UICutSubRectRatio(&Result, TopRatio, BottomRatio, LeftRatio, RightRatio);
    return Result;
}

static rectangle2
UIGetSubRectRatio(rectangle2* Rect, f32 Height, f32 Width) {
    rectangle2 Result = UIGetSubRectRatio(Rect, Height, Height, Width, Width);
    return Result;
} 
#endif //UI_RECT_CUT_H
