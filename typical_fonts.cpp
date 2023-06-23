#include "typical_fonts.h"

int getEngFontHeight(EngFonts f) {
    int res = 0;
    switch(f) {
        case EngFonts::Height16:
            res = 16;
            break;
        case EngFonts::Height20:
            res = 20;
            break;
        case EngFonts::Height25:
            res = 25;
            break;
        case EngFonts::Height28:
            res = 28;
            break;
        case EngFonts::Height36:
            res = 36;
            break;
        case EngFonts::Height49:
            res = 49;
            break;
        }
        return res;
}

int getCyrFontHeight(CyrFonts f) {
    int res = 0;
    switch(f) {
        case CyrFonts::Height20:
            res = 20;
            break;
        case CyrFonts::Height30:
            res = 30;
            break;
        case CyrFonts::Height40:
            res = 40;
            break;
    }
    return res;
}
