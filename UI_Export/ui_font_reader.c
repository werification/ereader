/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --bpp 1 --size 12 --font /usr/share/fonts/google-noto/NotoSans-Regular.ttf -r 0x20-0xFF,0x2000-0x206F,0x20A0-0x20CF --format lvgl -o /home/whyuntitledwhy/Documents/myereader/UI_Export/ui_font_reader.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef UI_FONT_READER
#define UI_FONT_READER 1
#endif

#if UI_FONT_READER

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0x80,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x14, 0x49, 0xf9, 0x22, 0x9f, 0xca, 0x24, 0x48,

    /* U+0024 "$" */
    0x23, 0xe9, 0x4e, 0x1c, 0xa5, 0xf1, 0x8,

    /* U+0025 "%" */
    0x62, 0x4b, 0x25, 0x13, 0xe9, 0xcb, 0xe4, 0x52,
    0x69, 0x23, 0x0,

    /* U+0026 "&" */
    0x30, 0x48, 0x48, 0x58, 0x30, 0xd2, 0x8a, 0x86,
    0x7b,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x4a, 0x49, 0x24, 0x89, 0x0,

    /* U+0029 ")" */
    0x48, 0x92, 0x49, 0x29, 0x0,

    /* U+002A "*" */
    0x10, 0x47, 0xcc, 0x28, 0x20,

    /* U+002B "+" */
    0x21, 0x3e, 0x42, 0x10,

    /* U+002C "," */
    0x58,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x11, 0x22, 0x24, 0x44, 0x80,

    /* U+0030 "0" */
    0x7b, 0x28, 0x61, 0x86, 0x18, 0x52, 0x78,

    /* U+0031 "1" */
    0x3e, 0x92, 0x49, 0x20,

    /* U+0032 "2" */
    0x72, 0x20, 0x82, 0x10, 0xc6, 0x10, 0xfc,

    /* U+0033 "3" */
    0x79, 0x10, 0x43, 0x30, 0x30, 0x61, 0x78,

    /* U+0034 "4" */
    0x8, 0x62, 0x8a, 0x4a, 0x2f, 0xc2, 0x8,

    /* U+0035 "5" */
    0x79, 0x4, 0x1e, 0xc, 0x10, 0x63, 0x78,

    /* U+0036 "6" */
    0x39, 0x8, 0x2e, 0xce, 0x18, 0x53, 0x78,

    /* U+0037 "7" */
    0xfc, 0x10, 0x82, 0x10, 0x42, 0x8, 0x60,

    /* U+0038 "8" */
    0x72, 0x28, 0xb4, 0x72, 0x38, 0x61, 0x78,

    /* U+0039 "9" */
    0x7b, 0x28, 0x61, 0x8d, 0xd0, 0x42, 0x70,

    /* U+003A ":" */
    0xc2,

    /* U+003B ";" */
    0xc3, 0x80,

    /* U+003C "<" */
    0x4, 0x66, 0x20, 0x60, 0x60, 0x40,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0x81, 0x3, 0x3, 0x33, 0x0, 0x0,

    /* U+003F "?" */
    0xe1, 0x11, 0x24, 0x40, 0x40,

    /* U+0040 "@" */
    0x1e, 0x30, 0x97, 0xf6, 0x5a, 0x2d, 0x16, 0x8b,
    0xba, 0x60, 0x1f, 0x0,

    /* U+0041 "A" */
    0x18, 0x18, 0x28, 0x24, 0x24, 0x7c, 0x42, 0x42,
    0x82,

    /* U+0042 "B" */
    0xfa, 0x18, 0x61, 0xfa, 0x18, 0x61, 0xf8,

    /* U+0043 "C" */
    0x3d, 0x8, 0x20, 0x82, 0x8, 0x10, 0x3c,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0x60, 0xc2, 0xf8,

    /* U+0045 "E" */
    0xfc, 0x21, 0xf, 0x42, 0x10, 0xf8,

    /* U+0046 "F" */
    0xfc, 0x21, 0xf, 0x42, 0x10, 0x80,

    /* U+0047 "G" */
    0x3e, 0x82, 0x4, 0x8, 0xf0, 0x60, 0xa1, 0x3e,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1, 0x82,

    /* U+0049 "I" */
    0xe9, 0x24, 0x92, 0xe0,

    /* U+004A "J" */
    0x24, 0x92, 0x49, 0x24, 0xe0,

    /* U+004B "K" */
    0x8e, 0x69, 0x28, 0xe2, 0xc9, 0x22, 0x84,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0xf8,

    /* U+004D "M" */
    0xc1, 0xe0, 0xe8, 0xb4, 0x5a, 0x2c, 0xa6, 0x53,
    0x39, 0x88, 0x80,

    /* U+004E "N" */
    0xc3, 0x86, 0x8d, 0x19, 0x33, 0x62, 0xc3, 0x86,

    /* U+004F "O" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81, 0x42,
    0x3c,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10, 0x80,

    /* U+0051 "Q" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81, 0x42,
    0x3c, 0x4, 0x2,

    /* U+0052 "R" */
    0xf2, 0x28, 0xa2, 0xf2, 0x49, 0x22, 0x88,

    /* U+0053 "S" */
    0x7c, 0x21, 0x87, 0xc, 0x21, 0xf0,

    /* U+0054 "T" */
    0xfc, 0x41, 0x4, 0x10, 0x41, 0x4, 0x10,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xe3, 0x7c,

    /* U+0056 "V" */
    0x82, 0x85, 0x12, 0x22, 0x45, 0xa, 0xc, 0x10,

    /* U+0057 "W" */
    0x84, 0x28, 0xcd, 0x29, 0x25, 0x24, 0xa4, 0x62,
    0x8c, 0x61, 0x8c, 0x31, 0x80,

    /* U+0058 "X" */
    0x44, 0x88, 0xa1, 0xc1, 0x5, 0xa, 0x22, 0xc6,

    /* U+0059 "Y" */
    0xc6, 0x89, 0xb1, 0x43, 0x82, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xfc, 0x30, 0x84, 0x30, 0x84, 0x30, 0xfc,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x93, 0x80,

    /* U+005C "\\" */
    0x84, 0x44, 0x22, 0x21, 0x10,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x27, 0x80,

    /* U+005E "^" */
    0x10, 0xc2, 0x92, 0x45, 0x10,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0x44,

    /* U+0061 "a" */
    0x70, 0x42, 0xf8, 0xc5, 0xe0,

    /* U+0062 "b" */
    0x82, 0x8, 0x2e, 0xce, 0x18, 0x61, 0xce, 0xe0,

    /* U+0063 "c" */
    0x7e, 0x21, 0x8, 0x61, 0xe0,

    /* U+0064 "d" */
    0x4, 0x10, 0x5d, 0xce, 0x18, 0x61, 0xcd, 0xd0,

    /* U+0065 "e" */
    0x76, 0x63, 0xf8, 0x61, 0xe0,

    /* U+0066 "f" */
    0x34, 0x4f, 0x44, 0x44, 0x44,

    /* U+0067 "g" */
    0x77, 0x38, 0x61, 0x87, 0x37, 0x41, 0x8f, 0xe0,

    /* U+0068 "h" */
    0x84, 0x21, 0x6c, 0xc6, 0x31, 0x8c, 0x40,

    /* U+0069 "i" */
    0x9f, 0xc0,

    /* U+006A "j" */
    0x20, 0x12, 0x49, 0x24, 0x9e,

    /* U+006B "k" */
    0x84, 0x21, 0x39, 0x53, 0x96, 0x94, 0x40,

    /* U+006C "l" */
    0xff, 0xc0,

    /* U+006D "m" */
    0xf7, 0x64, 0x62, 0x31, 0x18, 0x8c, 0x46, 0x22,

    /* U+006E "n" */
    0xf6, 0x63, 0x18, 0xc6, 0x20,

    /* U+006F "o" */
    0x7b, 0x38, 0x61, 0x87, 0x37, 0x80,

    /* U+0070 "p" */
    0xbb, 0x38, 0x61, 0x87, 0x3b, 0xa0, 0x82, 0x0,

    /* U+0071 "q" */
    0x77, 0x38, 0x61, 0x87, 0x37, 0x41, 0x4, 0x10,

    /* U+0072 "r" */
    0xbc, 0x88, 0x88, 0x80,

    /* U+0073 "s" */
    0x7c, 0x30, 0x60, 0x87, 0xc0,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x44, 0x70,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xcd, 0xe0,

    /* U+0076 "v" */
    0x85, 0x34, 0x92, 0x38, 0xc3, 0x0,

    /* U+0077 "w" */
    0x88, 0xe6, 0x55, 0x2a, 0xa5, 0x32, 0x98, 0x8c,

    /* U+0078 "x" */
    0x45, 0x23, 0xc, 0x39, 0x24, 0x40,

    /* U+0079 "y" */
    0x85, 0x34, 0x92, 0x38, 0xc3, 0x8, 0x23, 0x0,

    /* U+007A "z" */
    0xf8, 0x8c, 0x44, 0x63, 0xe0,

    /* U+007B "{" */
    0x29, 0x25, 0x12, 0x48, 0x80,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0x89, 0x24, 0x52, 0x4a, 0x0,

    /* U+007E "~" */
    0xe0, 0x70,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xbf,

    /* U+00A2 "¢" */
    0x23, 0xf1, 0x8, 0x43, 0xf, 0x20,

    /* U+00A3 "£" */
    0x3d, 0x4, 0x10, 0xf9, 0x4, 0x10, 0xfc,

    /* U+00A4 "¤" */
    0xfc, 0x63, 0x1f, 0x80,

    /* U+00A5 "¥" */
    0xc6, 0x89, 0xb1, 0x43, 0x8f, 0x9f, 0x8, 0x10,

    /* U+00A6 "¦" */
    0xf8, 0xf8,

    /* U+00A7 "§" */
    0x7c, 0x30, 0xe8, 0xc5, 0xc1, 0xf, 0x80,

    /* U+00A8 "¨" */
    0xa0,

    /* U+00A9 "©" */
    0x3e, 0x31, 0x97, 0x54, 0x1a, 0xd, 0x6, 0x82,
    0xba, 0x63, 0x1f, 0x0,

    /* U+00AA "ª" */
    0xfe, 0xf0,

    /* U+00AB "«" */
    0x2a, 0xa4, 0xa6, 0x80,

    /* U+00AC "¬" */
    0xfc, 0x10, 0x41,

    /* U+00AD "­" */
    0xe0,

    /* U+00AE "®" */
    0x3e, 0x31, 0x97, 0x52, 0x99, 0x4c, 0xc6, 0x52,
    0xaa, 0x63, 0x1f, 0x0,

    /* U+00AF "¯" */
    0xfc,

    /* U+00B0 "°" */
    0x69, 0x96,

    /* U+00B1 "±" */
    0x21, 0x9, 0xf2, 0x10, 0x1f,

    /* U+00B2 "²" */
    0x61, 0x12, 0x4f,

    /* U+00B3 "³" */
    0x61, 0x16, 0x1f,

    /* U+00B4 "´" */
    0x68,

    /* U+00B5 "µ" */
    0x8c, 0x63, 0x18, 0xcf, 0xf0, 0x84, 0x0,

    /* U+00B6 "¶" */
    0x3e, 0xf5, 0xef, 0xd7, 0xaf, 0x46, 0x85, 0xa,
    0x14, 0x28, 0x50,

    /* U+00B7 "·" */
    0x80,

    /* U+00B8 "¸" */
    0x9c,

    /* U+00B9 "¹" */
    0x2c, 0x92, 0x40,

    /* U+00BA "º" */
    0x69, 0x99, 0x60,

    /* U+00BB "»" */
    0x91, 0x62, 0x92, 0xd0,

    /* U+00BC "¼" */
    0x62, 0x51, 0x9, 0x4, 0x82, 0x90, 0x98, 0x54,
    0x4f, 0x21, 0x0,

    /* U+00BD "½" */
    0x62, 0x11, 0x9, 0x5, 0x72, 0x88, 0x84, 0x44,
    0x44, 0x23, 0x80,

    /* U+00BE "¾" */
    0x71, 0x4, 0x46, 0x20, 0x50, 0x14, 0xba, 0x60,
    0xa8, 0x4f, 0x20, 0x80,

    /* U+00BF "¿" */
    0x20, 0x22, 0x48, 0x88, 0x70,

    /* U+00C0 "À" */
    0x10, 0x10, 0x0, 0x18, 0x18, 0x28, 0x24, 0x24,
    0x7c, 0x42, 0x42, 0x82,

    /* U+00C1 "Á" */
    0x8, 0x10, 0x0, 0x18, 0x18, 0x28, 0x24, 0x24,
    0x7c, 0x42, 0x42, 0x82,

    /* U+00C2 "Â" */
    0x18, 0x2c, 0x0, 0x18, 0x18, 0x28, 0x24, 0x24,
    0x7c, 0x42, 0x42, 0x82,

    /* U+00C3 "Ã" */
    0x34, 0x4c, 0x0, 0x18, 0x18, 0x28, 0x24, 0x24,
    0x7c, 0x42, 0x42, 0x82,

    /* U+00C4 "Ä" */
    0x2c, 0x0, 0x0, 0x18, 0x18, 0x28, 0x24, 0x24,
    0x7c, 0x42, 0x42, 0x82,

    /* U+00C5 "Å" */
    0x1c, 0xa, 0x7, 0x1, 0x1, 0x40, 0xa0, 0x88,
    0x7c, 0x41, 0x20, 0x90, 0x40,

    /* U+00C6 "Æ" */
    0x1f, 0xc5, 0x1, 0x40, 0x90, 0x27, 0x9f, 0x4,
    0x42, 0x10, 0x87, 0xc0,

    /* U+00C7 "Ç" */
    0x3d, 0x8, 0x20, 0x82, 0x8, 0x10, 0x3c, 0x40,
    0x8e,

    /* U+00C8 "È" */
    0x41, 0x1, 0xf8, 0x42, 0x1e, 0x84, 0x21, 0xf0,

    /* U+00C9 "É" */
    0x11, 0x1, 0xf8, 0x42, 0x1e, 0x84, 0x21, 0xf0,

    /* U+00CA "Ê" */
    0x22, 0x81, 0xf8, 0x42, 0x1e, 0x84, 0x21, 0xf0,

    /* U+00CB "Ë" */
    0x50, 0x1, 0xf8, 0x42, 0x1e, 0x84, 0x21, 0xf0,

    /* U+00CC "Ì" */
    0x88, 0x74, 0x92, 0x49, 0x70,

    /* U+00CD "Í" */
    0x28, 0x74, 0x92, 0x49, 0x70,

    /* U+00CE "Î" */
    0x4a, 0xe, 0x44, 0x44, 0x44, 0x4e,

    /* U+00CF "Ï" */
    0x90, 0x7, 0x22, 0x22, 0x22, 0x27,

    /* U+00D0 "Ð" */
    0x7c, 0x42, 0x41, 0x41, 0xf1, 0x41, 0x41, 0x42,
    0x7c,

    /* U+00D1 "Ñ" */
    0x34, 0xb0, 0x6, 0x1c, 0x34, 0x68, 0xc9, 0x9b,
    0x16, 0x1c, 0x30,

    /* U+00D2 "Ò" */
    0x10, 0x8, 0x0, 0x3c, 0x42, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+00D3 "Ó" */
    0xc, 0x8, 0x0, 0x3c, 0x42, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+00D4 "Ô" */
    0x18, 0x24, 0x0, 0x3c, 0x42, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+00D5 "Õ" */
    0x34, 0x4c, 0x0, 0x3c, 0x42, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+00D6 "Ö" */
    0x28, 0x0, 0x0, 0x3c, 0x42, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x3c,

    /* U+00D7 "×" */
    0x8a, 0x88, 0xa8, 0x80,

    /* U+00D8 "Ø" */
    0x3e, 0x42, 0x85, 0x89, 0x99, 0x91, 0xa1, 0x42,
    0x7c,

    /* U+00D9 "Ù" */
    0x20, 0x20, 0x4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xe0,

    /* U+00DA "Ú" */
    0x8, 0x20, 0x4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xe0,

    /* U+00DB "Û" */
    0x10, 0x50, 0x4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xe0,

    /* U+00DC "Ü" */
    0x28, 0x0, 0x4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xe0,

    /* U+00DD "Ý" */
    0x8, 0x20, 0x6, 0x34, 0x4d, 0x8a, 0x1c, 0x10,
    0x20, 0x40, 0x80,

    /* U+00DE "Þ" */
    0x87, 0xa3, 0x18, 0xcf, 0xd0, 0x80,

    /* U+00DF "ß" */
    0x72, 0x28, 0xa4, 0xa2, 0x48, 0xa1, 0x86, 0xe0,

    /* U+00E0 "à" */
    0x61, 0x0, 0xe0, 0x85, 0xf1, 0x8b, 0xc0,

    /* U+00E1 "á" */
    0x11, 0x0, 0xe0, 0x85, 0xf1, 0x8b, 0xc0,

    /* U+00E2 "â" */
    0x22, 0x80, 0xe0, 0x85, 0xf1, 0x8b, 0xc0,

    /* U+00E3 "ã" */
    0x78, 0x1c, 0x10, 0xbe, 0x31, 0x78,

    /* U+00E4 "ä" */
    0x50, 0x1c, 0x10, 0xbe, 0x31, 0x78,

    /* U+00E5 "å" */
    0x72, 0x9c, 0x7, 0x4, 0x2f, 0x8c, 0x5e,

    /* U+00E6 "æ" */
    0x77, 0x4, 0x42, 0x2f, 0xf8, 0x84, 0x41, 0xde,

    /* U+00E7 "ç" */
    0x7e, 0x21, 0x8, 0x61, 0xe4, 0x13, 0x80,

    /* U+00E8 "è" */
    0x41, 0x0, 0xec, 0xc7, 0xf0, 0xc3, 0xc0,

    /* U+00E9 "é" */
    0x11, 0x0, 0xec, 0xc7, 0xf0, 0xc3, 0xc0,

    /* U+00EA "ê" */
    0x22, 0x80, 0xec, 0xc7, 0xf0, 0xc3, 0xc0,

    /* U+00EB "ë" */
    0x50, 0x1d, 0x98, 0xfe, 0x18, 0x78,

    /* U+00EC "ì" */
    0x91, 0x55, 0x50,

    /* U+00ED "í" */
    0x62, 0xaa, 0xa0,

    /* U+00EE "î" */
    0x22, 0x80, 0x42, 0x10, 0x84, 0x21, 0x0,

    /* U+00EF "ï" */
    0xa1, 0x24, 0x92, 0x40,

    /* U+00F0 "ð" */
    0x0, 0xe6, 0x82, 0x77, 0x38, 0x61, 0xcd, 0xe0,

    /* U+00F1 "ñ" */
    0x70, 0x3d, 0x98, 0xc6, 0x31, 0x88,

    /* U+00F2 "ò" */
    0x20, 0xc0, 0x1e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F3 "ó" */
    0x18, 0x40, 0x1e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F4 "ô" */
    0x31, 0x20, 0x1e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F5 "õ" */
    0x78, 0x7, 0xb3, 0x86, 0x18, 0x73, 0x78,

    /* U+00F6 "ö" */
    0x50, 0x7, 0xb3, 0x86, 0x18, 0x73, 0x78,

    /* U+00F7 "÷" */
    0x20, 0x1, 0xf0, 0x10,

    /* U+00F8 "ø" */
    0x7f, 0x39, 0x6d, 0xa7, 0x3f, 0x80,

    /* U+00F9 "ù" */
    0x41, 0x1, 0x18, 0xc6, 0x31, 0x9b, 0xc0,

    /* U+00FA "ú" */
    0x11, 0x1, 0x18, 0xc6, 0x31, 0x9b, 0xc0,

    /* U+00FB "û" */
    0x22, 0x81, 0x18, 0xc6, 0x31, 0x9b, 0xc0,

    /* U+00FC "ü" */
    0x50, 0x23, 0x18, 0xc6, 0x33, 0x78,

    /* U+00FD "ý" */
    0x18, 0x40, 0x21, 0x4d, 0x24, 0x8e, 0x30, 0xc2,
    0x8, 0xc0,

    /* U+00FE "þ" */
    0x82, 0x8, 0x2e, 0xce, 0x18, 0x61, 0xce, 0xe8,
    0x20, 0x80,

    /* U+00FF "ÿ" */
    0x68, 0x8, 0x53, 0x49, 0x23, 0x8c, 0x30, 0x82,
    0x30,

    /* U+2000 " " */
    0x0,

    /* U+2001 " " */
    0x0,

    /* U+2002 " " */
    0x0,

    /* U+2003 " " */
    0x0,

    /* U+2004 " " */
    0x0,

    /* U+2005 " " */
    0x0,

    /* U+2006 " " */
    0x0,

    /* U+2007 " " */
    0x0,

    /* U+2008 " " */
    0x0,

    /* U+2009 " " */
    0x0,

    /* U+200A " " */
    0x0,

    /* U+200B "​" */
    0x0,

    /* U+200C "‌" */
    0x0,

    /* U+200D "‍" */
    0x0,

    /* U+200E "‎" */
    0x3e, 0xc9, 0x24, 0x92, 0x0,

    /* U+200F "‏" */
    0x9e, 0x92, 0x49, 0x24, 0x80,

    /* U+2010 "‐" */
    0xe0,

    /* U+2011 "‑" */
    0xe0,

    /* U+2012 "‒" */
    0xfc,

    /* U+2013 "–" */
    0xf8,

    /* U+2014 "—" */
    0xff, 0xe0,

    /* U+2015 "―" */
    0xff, 0xe0,

    /* U+2016 "‖" */
    0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x90,

    /* U+2017 "‗" */
    0xf8, 0x3e,

    /* U+2018 "‘" */
    0x58,

    /* U+2019 "’" */
    0x78,

    /* U+201A "‚" */
    0x58,

    /* U+201B "‛" */
    0x94,

    /* U+201C "“" */
    0x55, 0xb0,

    /* U+201D "”" */
    0x5d, 0xa0,

    /* U+201E "„" */
    0x55, 0x90,

    /* U+201F "‟" */
    0xb5, 0x50,

    /* U+2020 "†" */
    0x21, 0x9, 0xf2, 0x10, 0x84, 0x21, 0x0,

    /* U+2021 "‡" */
    0x21, 0x3e, 0x42, 0x10, 0x9f, 0x21, 0x0,

    /* U+2022 "•" */
    0xff, 0x80,

    /* U+2023 "‣" */
    0x1b, 0xc0,

    /* U+2024 "․" */
    0x80,

    /* U+2025 "‥" */
    0x90,

    /* U+2026 "…" */
    0x92,

    /* U+2027 "‧" */
    0x80,

    /* U+2028 " " */
    0x0,

    /* U+2029 " " */
    0x0,

    /* U+202A "‪" */
    0xf2, 0x49, 0x24, 0x90,

    /* U+202B "‫" */
    0xe4, 0x92, 0x49, 0x24,

    /* U+202C "‬" */
    0xfd, 0x24, 0x92, 0x48,

    /* U+202D "‭" */
    0xf3, 0xa4, 0x92, 0x49, 0x0,

    /* U+202E "‮" */
    0xe7, 0xa4, 0x92, 0x49, 0x0,

    /* U+202F " " */
    0x0,

    /* U+2030 "‰" */
    0x62, 0x4, 0xb0, 0x25, 0x1, 0x3e, 0x69, 0xcc,
    0xbe, 0x64, 0x53, 0x26, 0x99, 0x23, 0x30,

    /* U+2031 "‱" */
    0x62, 0x0, 0x4a, 0x0, 0x25, 0x0, 0x13, 0x62,
    0x29, 0xca, 0xab, 0xe5, 0x54, 0x52, 0xaa, 0x29,
    0x55, 0x23, 0x11, 0x0,

    /* U+2032 "′" */
    0x29, 0x40,

    /* U+2033 "″" */
    0x2a, 0xa4,

    /* U+2034 "‴" */
    0x2a, 0xaa, 0x50,

    /* U+2035 "‵" */
    0xa4,

    /* U+2036 "‶" */
    0xa5, 0x50,

    /* U+2037 "‷" */
    0xa9, 0x65, 0x40,

    /* U+2038 "‸" */
    0x23, 0x15, 0x20,

    /* U+2039 "‹" */
    0x2a, 0x26,

    /* U+203A "›" */
    0x88, 0xac,

    /* U+203B "※" */
    0x88, 0xa0, 0x88, 0x82, 0x88, 0x88, 0xa0, 0x88,
    0x82, 0x88, 0x80,

    /* U+203C "‼" */
    0x99, 0x99, 0x99, 0x0, 0x90,

    /* U+203D "‽" */
    0xe5, 0x55, 0x64, 0x40, 0x40,

    /* U+203E "‾" */
    0xfc,

    /* U+203F "‿" */
    0x82, 0x88, 0xe0,

    /* U+2040 "⁀" */
    0x38, 0x8a, 0x8,

    /* U+2041 "⁁" */
    0x11, 0x22, 0x26, 0x69,

    /* U+2042 "⁂" */
    0x2, 0x0, 0x10, 0x3, 0xe0, 0x4, 0x0, 0x50,
    0x2, 0xc0, 0xc0, 0x84, 0x4, 0xfc, 0xf9, 0x81,
    0x1e, 0x14, 0x0, 0xa0,

    /* U+2043 "⁃" */
    0xe0,

    /* U+2044 "⁄" */
    0x4, 0x21, 0x4, 0x20, 0x84, 0x20, 0x80,

    /* U+2045 "⁅" */
    0xf2, 0x49, 0xe4, 0x93, 0x80,

    /* U+2046 "⁆" */
    0xe4, 0x93, 0xc9, 0x27, 0x80,

    /* U+2047 "⁇" */
    0xe7, 0x84, 0x11, 0x4, 0x43, 0x21, 0x90, 0x84,
    0x20, 0x0, 0x42, 0x0,

    /* U+2048 "⁈" */
    0xe2, 0x24, 0x49, 0x14, 0x28, 0x40, 0x0, 0x42,

    /* U+2049 "⁉" */
    0xbd, 0x6, 0xc, 0x28, 0x91, 0x0, 0x0, 0x88,

    /* U+204A "⁊" */
    0xfc, 0x10, 0x86, 0x10, 0x82, 0x0,

    /* U+204B "⁋" */
    0xfa, 0xfb, 0xef, 0xbe, 0xfb, 0x28, 0xa2, 0x8a,
    0x28,

    /* U+204C "⁌" */
    0x7f, 0xdf, 0x7d, 0xf7, 0xd7, 0xc0,

    /* U+204D "⁍" */
    0xfa, 0xfb, 0xef, 0xbe, 0xff, 0x80,

    /* U+204E "⁎" */
    0x10, 0x47, 0xcc, 0x28, 0x20,

    /* U+204F "⁏" */
    0xc3, 0x80,

    /* U+2050 "⁐" */
    0x38, 0x8a, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x2, 0xa, 0x23, 0x80,

    /* U+2051 "⁑" */
    0x10, 0x47, 0xcc, 0x28, 0x21, 0x4, 0x15, 0xf3,
    0x9a,

    /* U+2052 "⁒" */
    0x92, 0x22, 0x44, 0x48, 0x90,

    /* U+2053 "⁓" */
    0x78, 0x61, 0xe0,

    /* U+2054 "⁔" */
    0x38, 0x8a, 0x8,

    /* U+2055 "⁕" */
    0x54, 0x73, 0xf9, 0xc5, 0x42, 0x0,

    /* U+2056 "⁖" */
    0x8, 0x0, 0x8, 0x0, 0x0, 0x8,

    /* U+2057 "⁗" */
    0x2a, 0xaa, 0xa5, 0x40,

    /* U+2058 "⁘" */
    0x8, 0x0, 0x0, 0x0, 0x81, 0x0, 0x0, 0x0,
    0x8,

    /* U+2059 "⁙" */
    0x81, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0,
    0x81,

    /* U+205A "⁚" */
    0x80, 0x80,

    /* U+205B "⁛" */
    0x10, 0x0, 0x0, 0x0, 0x10, 0x40, 0x0, 0x0,
    0x20,

    /* U+205C "⁜" */
    0x8, 0x4, 0x12, 0x41, 0x0, 0x87, 0xfc, 0x20,
    0x10, 0x49, 0x4, 0x0,

    /* U+205D "⁝" */
    0x88, 0x80,

    /* U+205E "⁞" */
    0xa4, 0x80,

    /* U+205F " " */
    0x0,

    /* U+2060 "⁠" */
    0x0,

    /* U+2061 "⁡" */
    0x0,

    /* U+2062 "⁢" */
    0x0,

    /* U+2063 "⁣" */
    0x0,

    /* U+2064 "⁤" */
    0x0,

    /* U+2066 "⁦" */
    0x0,

    /* U+2067 "⁧" */
    0x0,

    /* U+2068 "⁨" */
    0x0,

    /* U+2069 "⁩" */
    0x0,

    /* U+206A "⁪" */
    0xeb, 0xa4, 0x92, 0x49, 0x0,

    /* U+206B "⁫" */
    0xf7, 0xa4, 0x92, 0x49, 0x0,

    /* U+206C "⁬" */
    0xfd, 0x24, 0x92, 0x49, 0x0,

    /* U+206D "⁭" */
    0x4b, 0xa4, 0x92, 0x49, 0x0,

    /* U+206E "⁮" */
    0x57, 0x24, 0x92, 0x49, 0x0,

    /* U+206F "⁯" */
    0xcb, 0xa4, 0x92, 0x49, 0x0,

    /* U+20A0 "₠" */
    0x73, 0x8, 0x27, 0x91, 0xf1, 0x4, 0x1c,

    /* U+20A1 "₡" */
    0x18, 0xf6, 0xaa, 0xb2, 0xcd, 0x34, 0x7d, 0x44,
    0x0,

    /* U+20A2 "₢" */
    0x3d, 0x8, 0x27, 0x92, 0x49, 0x14, 0x3c,

    /* U+20A3 "₣" */
    0x7d, 0x4, 0x10, 0x79, 0xf, 0x10, 0x40,

    /* U+20A4 "₤" */
    0x3d, 0x4, 0x3e, 0x43, 0xe4, 0x10, 0xfc,

    /* U+20A5 "₥" */
    0x2, 0x2, 0x3d, 0xd9, 0x18, 0x8c, 0x46, 0x63,
    0x31, 0xa8, 0x90, 0x0, 0x0,

    /* U+20A6 "₦" */
    0x64, 0xc9, 0x97, 0xf5, 0x5f, 0xd3, 0x26, 0x4c,

    /* U+20A7 "₧" */
    0xf0, 0x88, 0x8a, 0x9a, 0xf7, 0x82, 0x82, 0x82,
    0x83,

    /* U+20A8 "₨" */
    0xe0, 0x48, 0x24, 0xf2, 0x8e, 0x45, 0x1a, 0xc3,
    0x21, 0x97, 0x0,

    /* U+20A9 "₩" */
    0x99, 0x99, 0x5a, 0x5a, 0xff, 0x66, 0x66, 0x66,
    0x26,

    /* U+20AA "₪" */
    0xf1, 0x89, 0xa9, 0xa9, 0xa9, 0xa9, 0xa9, 0xa3,
    0xbe,

    /* U+20AB "₫" */
    0x4, 0x7c, 0x13, 0xac, 0xd0, 0xa1, 0x42, 0xcc,
    0xe8, 0x3, 0xe0,

    /* U+20AC "€" */
    0x3d, 0x84, 0x3e, 0x43, 0xc4, 0x18, 0x3c,

    /* U+20AD "₭" */
    0x44, 0x91, 0x62, 0x8f, 0xca, 0x12, 0x26, 0x44,

    /* U+20AE "₮" */
    0xfc, 0x41, 0x6, 0x31, 0x63, 0x14, 0x10,

    /* U+20AF "₯" */
    0xf0, 0x1, 0x1, 0x10, 0x22, 0x4, 0x40, 0x8b,
    0xf2, 0x4c, 0xd9, 0xf3, 0xc0, 0x40, 0x8, 0x1,
    0x0,

    /* U+20B0 "₰" */
    0x1c, 0x51, 0x45, 0x18, 0x24, 0x9e, 0x64, 0x10,
    0x43,

    /* U+20B1 "₱" */
    0x78, 0x9b, 0xff, 0xf4, 0x8f, 0x10, 0x20, 0x40,

    /* U+20B2 "₲" */
    0x8, 0x7d, 0xa6, 0x48, 0x91, 0xe2, 0xc5, 0x6a,
    0x7c, 0x20,

    /* U+20B3 "₳" */
    0x10, 0x50, 0xa7, 0xf6, 0x5f, 0xd1, 0x61, 0x82,

    /* U+20B4 "₴" */
    0xf0, 0x20, 0xbf, 0x63, 0xf8, 0x20, 0x78,

    /* U+20B5 "₵" */
    0x10, 0xf5, 0x24, 0x92, 0x49, 0x24, 0x51, 0xf1,
    0x0,

    /* U+20B6 "₶" */
    0x49, 0xfd, 0x22, 0x44, 0x89, 0x12, 0x27,

    /* U+20B7 "₷" */
    0xf, 0x10, 0x10, 0x18, 0x6, 0x1, 0xfd, 0xd5,
    0x7e, 0x54,

    /* U+20B8 "₸" */
    0xfc, 0xf, 0xc4, 0x10, 0x41, 0x4, 0x10,

    /* U+20B9 "₹" */
    0xf9, 0x3e, 0x4e, 0x41, 0xc, 0x20,

    /* U+20BA "₺" */
    0x41, 0x6e, 0x1e, 0xc1, 0x14, 0x52, 0x70,

    /* U+20BB "₻" */
    0xe0, 0x50, 0x3f, 0xc4, 0x12, 0xe9, 0xac, 0x80,
    0x20, 0xe, 0x0,

    /* U+20BC "₼" */
    0x8, 0x4, 0x2, 0x7, 0xc4, 0x94, 0x46, 0x23,
    0x1, 0x80, 0x80,

    /* U+20BD "₽" */
    0x79, 0x14, 0x51, 0xf9, 0xf, 0x10, 0x40,

    /* U+20BE "₾" */
    0x18, 0x3c, 0x5a, 0x99, 0x99, 0x80, 0x80, 0xc0,
    0x40, 0x7f,

    /* U+20BF "₿" */
    0x52, 0xbd, 0x18, 0xc7, 0xd1, 0x8c, 0x7c, 0xa0,

    /* U+20C0 "⃀" */
    0x7e, 0x21, 0x8, 0x61, 0xe0, 0xf8
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 50, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 52, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 78, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 5, .adv_w = 124, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 110, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 20, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 141, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 43, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 41, .adv_w = 58, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 46, .adv_w = 58, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 51, .adv_w = 106, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 56, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 60, .adv_w = 51, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 61, .adv_w = 62, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 62, .adv_w = 51, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 71, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 110, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 93, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 51, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 136, .adv_w = 51, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 138, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 144, .adv_w = 110, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 147, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 153, .adv_w = 83, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 173, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 170, .adv_w = 123, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 125, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 121, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 107, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 100, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 221, .adv_w = 142, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 65, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 52, .box_w = 3, .box_h = 12, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 238, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 101, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 174, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 146, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 150, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 279, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 150, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 296, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 105, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 107, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 179, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 113, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 109, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 361, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 63, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 373, .adv_w = 71, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 63, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 383, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 388, .adv_w = 85, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 389, .adv_w = 54, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 390, .adv_w = 108, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 92, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 416, .adv_w = 108, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 66, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 426, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 434, .adv_w = 119, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 50, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 443, .adv_w = 50, .box_w = 3, .box_h = 13, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 448, .adv_w = 103, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 455, .adv_w = 50, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 180, .box_w = 9, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 465, .adv_w = 119, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 484, .adv_w = 118, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 492, .adv_w = 79, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 92, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 69, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 506, .adv_w = 119, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 517, .adv_w = 151, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 102, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 531, .adv_w = 98, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 539, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 544, .adv_w = 73, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 549, .adv_w = 106, .box_w = 1, .box_h = 13, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 551, .adv_w = 73, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 556, .adv_w = 110, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 558, .adv_w = 50, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 52, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 560, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 566, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 110, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 577, .adv_w = 110, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 106, .box_w = 1, .box_h = 13, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 587, .adv_w = 98, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 594, .adv_w = 111, .box_w = 3, .box_h = 1, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 595, .adv_w = 160, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 69, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 609, .adv_w = 98, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 613, .adv_w = 110, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 616, .adv_w = 62, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 617, .adv_w = 160, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 629, .adv_w = 96, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = 10},
    {.bitmap_index = 630, .adv_w = 82, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 632, .adv_w = 110, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 67, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 640, .adv_w = 67, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 643, .adv_w = 54, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 644, .adv_w = 120, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 651, .adv_w = 126, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 662, .adv_w = 51, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 663, .adv_w = 43, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 664, .adv_w = 67, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 667, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 670, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 674, .adv_w = 143, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 696, .adv_w = 150, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 83, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 713, .adv_w = 123, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 123, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 737, .adv_w = 123, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 749, .adv_w = 123, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 761, .adv_w = 123, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 773, .adv_w = 123, .box_w = 9, .box_h = 11, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 786, .adv_w = 169, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 798, .adv_w = 121, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 807, .adv_w = 107, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 815, .adv_w = 107, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 823, .adv_w = 107, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 107, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 839, .adv_w = 65, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 65, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 849, .adv_w = 65, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 855, .adv_w = 65, .box_w = 4, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 861, .adv_w = 140, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 870, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 881, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 893, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 905, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 917, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 929, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 941, .adv_w = 110, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 945, .adv_w = 150, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 954, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 965, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 976, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 987, .adv_w = 140, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 998, .adv_w = 109, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1009, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1015, .adv_w = 121, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1023, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1030, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1037, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1044, .adv_w = 108, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1050, .adv_w = 108, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1056, .adv_w = 108, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1063, .adv_w = 166, .box_w = 9, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1071, .adv_w = 92, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1078, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1085, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1092, .adv_w = 108, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1099, .adv_w = 108, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1105, .adv_w = 50, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1108, .adv_w = 50, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1111, .adv_w = 50, .box_w = 5, .box_h = 10, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 1118, .adv_w = 50, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1122, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1130, .adv_w = 119, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1136, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1144, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1152, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1160, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1167, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1174, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 1178, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1184, .adv_w = 119, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1191, .adv_w = 119, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1198, .adv_w = 119, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1205, .adv_w = 119, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1211, .adv_w = 98, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1221, .adv_w = 118, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1231, .adv_w = 98, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1240, .adv_w = 96, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1241, .adv_w = 192, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1242, .adv_w = 96, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1243, .adv_w = 192, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1244, .adv_w = 64, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1245, .adv_w = 48, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1246, .adv_w = 32, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1247, .adv_w = 110, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1248, .adv_w = 51, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1249, .adv_w = 32, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1250, .adv_w = 19, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1251, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1252, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1253, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1254, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1259, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -2, .ofs_y = -2},
    {.bitmap_index = 1264, .adv_w = 62, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1265, .adv_w = 62, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1266, .adv_w = 110, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 1267, .adv_w = 96, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1268, .adv_w = 192, .box_w = 11, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1270, .adv_w = 192, .box_w = 11, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1272, .adv_w = 106, .box_w = 4, .box_h = 13, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 1279, .adv_w = 79, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1281, .adv_w = 34, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1282, .adv_w = 34, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1283, .adv_w = 48, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1284, .adv_w = 34, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1285, .adv_w = 69, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1287, .adv_w = 69, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1289, .adv_w = 80, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1291, .adv_w = 69, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1293, .adv_w = 98, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1300, .adv_w = 98, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1307, .adv_w = 72, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1309, .adv_w = 71, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1311, .adv_w = 96, .box_w = 1, .box_h = 1, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1312, .adv_w = 103, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1313, .adv_w = 152, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1314, .adv_w = 51, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1315, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1316, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1317, .adv_w = 0, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1321, .adv_w = 0, .box_w = 3, .box_h = 10, .ofs_x = -2, .ofs_y = -2},
    {.bitmap_index = 1325, .adv_w = 0, .box_w = 3, .box_h = 10, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1329, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1334, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1339, .adv_w = 32, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1340, .adv_w = 226, .box_w = 13, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1355, .adv_w = 296, .box_w = 17, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1375, .adv_w = 45, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 1377, .adv_w = 78, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1379, .adv_w = 112, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1382, .adv_w = 45, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 1383, .adv_w = 84, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1385, .adv_w = 116, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1388, .adv_w = 77, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1391, .adv_w = 60, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1393, .adv_w = 60, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1395, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1406, .adv_w = 96, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1411, .adv_w = 85, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1416, .adv_w = 96, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = 10},
    {.bitmap_index = 1417, .adv_w = 117, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1420, .adv_w = 117, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 1423, .adv_w = 72, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1427, .adv_w = 215, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1447, .adv_w = 62, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1448, .adv_w = 25, .box_w = 6, .box_h = 9, .ofs_x = -2, .ofs_y = 0},
    {.bitmap_index = 1455, .adv_w = 62, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1460, .adv_w = 62, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1465, .adv_w = 164, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1477, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1485, .adv_w = 131, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1493, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1499, .adv_w = 126, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1508, .adv_w = 123, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1514, .adv_w = 123, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1520, .adv_w = 106, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1525, .adv_w = 46, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1527, .adv_w = 116, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1539, .adv_w = 106, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1548, .adv_w = 71, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1553, .adv_w = 192, .box_w = 10, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1556, .adv_w = 116, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1559, .adv_w = 110, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1565, .adv_w = 99, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1571, .adv_w = 142, .box_w = 9, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1575, .adv_w = 158, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1584, .adv_w = 159, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1593, .adv_w = 42, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1595, .adv_w = 126, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1604, .adv_w = 153, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1616, .adv_w = 51, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1618, .adv_w = 51, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1620, .adv_w = 43, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1621, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1622, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1623, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1624, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1625, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1626, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1627, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1628, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1629, .adv_w = 0, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1630, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1635, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1640, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1645, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1650, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1655, .adv_w = 0, .box_w = 3, .box_h = 11, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1660, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1667, .adv_w = 110, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1676, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1683, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1690, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1697, .adv_w = 180, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1710, .adv_w = 110, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1718, .adv_w = 149, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1727, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1738, .adv_w = 129, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1747, .adv_w = 153, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1756, .adv_w = 118, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1767, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1774, .adv_w = 110, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1782, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1789, .adv_w = 186, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1806, .adv_w = 110, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1815, .adv_w = 110, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1823, .adv_w = 140, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1833, .adv_w = 117, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1841, .adv_w = 105, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1848, .adv_w = 121, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1857, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1864, .adv_w = 139, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1874, .adv_w = 105, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1881, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1887, .adv_w = 110, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1894, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1905, .adv_w = 156, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1916, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1923, .adv_w = 148, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1933, .adv_w = 110, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1941, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 96, .glyph_id_start = 96,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8192, .range_length = 101, .glyph_id_start = 192,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8294, .range_length = 10, .glyph_id_start = 293,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8352, .range_length = 33, .glyph_id_start = 303,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 2, 0, 0, 0, 3, 4, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 5, 6, 7, 8, 9, 10,
    0, 0, 0, 0, 7, 11, 0, 0,
    8, 12, 8, 13, 0, 14, 15, 16,
    16, 7, 17, 18, 2, 0, 0, 0,
    19, 0, 20, 21, 22, 0, 21, 23,
    0, 20, 0, 0, 0, 0, 20, 20,
    21, 21, 0, 24, 0, 22, 0, 25,
    25, 26, 25, 0, 2, 0, 0, 0,
    0, 27, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 28, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 29, 0, 0, 0, 30,
    5, 5, 5, 5, 5, 5, 9, 7,
    9, 9, 9, 9, 0, 0, 0, 0,
    8, 0, 8, 8, 8, 8, 8, 0,
    8, 15, 15, 15, 15, 17, 12, 0,
    20, 20, 20, 20, 20, 20, 21, 0,
    21, 21, 21, 21, 0, 0, 31, 31,
    21, 0, 21, 21, 21, 21, 21, 0,
    21, 0, 0, 0, 0, 25, 21, 25,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 4, 0, 0,
    0, 0, 3, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 28, 29, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 2,
    1, 0, 3, 0, 0, 4, 5, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 0, 7, 0, 8, 0, 0, 0,
    8, 0, 0, 9, 0, 0, 0, 0,
    8, 0, 8, 0, 0, 10, 11, 12,
    12, 13, 14, 15, 0, 0, 3, 0,
    0, 0, 16, 0, 17, 17, 17, 0,
    18, 0, 0, 19, 0, 0, 20, 20,
    17, 20, 17, 20, 21, 0, 20, 22,
    22, 22, 22, 23, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 24, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 25, 0, 0, 0, 0,
    7, 7, 7, 7, 7, 7, 26, 8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 8, 8, 8, 8, 8, 0,
    8, 11, 11, 11, 11, 14, 0, 0,
    17, 16, 16, 16, 16, 16, 16, 17,
    17, 17, 17, 17, 0, 0, 0, 0,
    0, 0, 17, 17, 17, 17, 17, 0,
    17, 20, 20, 20, 20, 22, 0, 22,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 5, 5, 5, 0, 0,
    0, 1, 4, 0, 0, 1, 4, 0,
    0, 0, 0, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 24, 25, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, -4, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 17, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -10, 0, -13, -4, -12,
    0, -12, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, -4,
    10, -13, 0, -8, 0, -12, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -4, 0, 0, 0, 0,
    0, -8, 0, 0, -4, 0, 0, -6,
    0, -2, -4, -2, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    12, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4, -12, 0, 4,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -15, 0, 0, 0,
    0, 0, 0, -4, 0, -4, -2, -4,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, -25, 0, 0, -10, 0, 0, 0,
    0, 0, -4, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 0, 0, -4, 0, -12, -8, 4,
    -13, -4, 0, 4, 0, 0, 0, 0,
    0, -15, -13, -13, 0, -10, -12, -4,
    -8, -12, -6, 0, 0, 0, 0, -4,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 4, -8, -2, 0, 0,
    0, 0, 0, 0, 0, -4, -4, -2,
    0, -2, -2, 0, 0, -6, -2, 0,
    0, -6, 0, -12, 0, 4, -12, -4,
    0, 0, 0, 0, 0, 0, 0, -10,
    -10, -10, 0, -6, -8, 0, -4, -15,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 18, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, -2, 0,
    0, 0, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 12, 0, 8, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 8, 0,
    0, -31, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, -4, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    8, 0, 0, -8, 0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 10, -4, 0, -2,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, -2, 0, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, -6, -4, -15, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 19, -8, 0, -6, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 31,
    .right_class_cnt     = 26,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 5,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_reader = {
#else
lv_font_t ui_font_reader = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_READER*/

