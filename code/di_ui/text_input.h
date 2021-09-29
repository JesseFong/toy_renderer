/* date = July 22nd 2020 2:53 pm */

#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#endif //TEXT_INPUT_H

struct text_input {
    
#define MAX_TEXT_INPUT_COUNT 65536
    u8 Text[MAX_TEXT_INPUT_COUNT];
    u32 TextCount;
};

internal void
InputText(text_input* Input, u8* TextToEnter) {
    Input->Text[Input->TextCount] = *TextToEnter;
    Input->TextCount++;
}

internal void
DeleteText(text_input* Input, u32 DeleteCount) {
    while(DeleteCount--) {
        Input->Text[Input->TextCount - 1] = 0;
        Input->TextCount--;
    }
}