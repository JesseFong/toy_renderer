
static u32
CStringLength(char* String) {
    u32 Length = 0;
    for(;String[Length] != 0; Length++);
    return Length;
}

static string_u8
MakeStringU8(u8* Data, u32 Size) {
    string_u8 Result = {};
    Result.Data = Data;
    Result.Size = Size;
}

static string_u8
MakeStringU8(char* CString) {
    string_u8 Result = {};
    Result.Data;
    Result.Size = CStringLength(CString);
}
