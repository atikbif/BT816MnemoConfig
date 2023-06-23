#ifndef TYPICAL_FONTS_H
#define TYPICAL_FONTS_H

enum class EngFonts {Height16, Height20, Height25, Height28, Height36, Height49, LastEl};
enum class CyrFonts {Height20, Height30, Height40, LastEl};

int getEngFontHeight(EngFonts f);
int getCyrFontHeight(CyrFonts f);

#endif // TYPICAL_FONTS_H
