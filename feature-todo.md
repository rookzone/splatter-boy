# TODO


## Refactor

Factor out text / font code to seperate files


## Window

Window tile management

    Load window tile function
    track next free vram slot for window tiles
    Activate window box (pass tile rect, creates window from that location)
    Store unsigned char* windowMap / store font as a window map
    Store font start index in windowMap

    Print_text
        Creates window rect for size of string
        Load font data into window vram
        Places font tiles on top of window
        Turn window on / off
        Clear tiles from window vram

    clear_text
        Clear function removes font from window map at font_start_index of size sizeof(font)
        Shift vram to fill gap
        update window vram next available tile.

