/**
 * @file cp437_constants.h
 * @brief Code page 437 (CP437) character constants
 * @note C99-compatible enum for all 256 CP437 characters (0x00-0xFF)
 */

#ifndef CP437_CONSTANTS_H
#define CP437_CONSTANTS_H

typedef enum {
    // =============================================
    // Control Characters (0x00-0x1F) + IBM Symbols
    // =============================================
    CP437_NULL                   = 0x00,  // ^@ (Null)
    CP437_SMILING_FACE           = 0x01,  // ☺ (IBM: Smiling face)
    CP437_INVERSE_SMILING_FACE   = 0x02,  // ☻ (IBM: Inverse smiling face)
    CP437_HEART                  = 0x03,  // ♥ (IBM: Heart)
    CP437_DIAMOND                = 0x04,  // ♦ (IBM: Diamond)
    CP437_CLUB                   = 0x05,  // ♣ (IBM: Club)
    CP437_SPADE                  = 0x06,  // ♠ (IBM: Spade)
    CP437_BULLET                 = 0x07,  // • (IBM: Bullet)
    CP437_INVERSE_BULLET         = 0x08,  // ◘ (IBM: Inverse bullet)
    CP437_WHITE_CIRCLE           = 0x09,  // ○ (IBM: White circle)
    CP437_INVERSE_WHITE_CIRCLE   = 0x0A,  // ◙ (IBM: Inverse white circle)
    CP437_MALE_SYMBOL            = 0x0B,  // ♂ (IBM: Male symbol)
    CP437_FEMALE_SYMBOL          = 0x0C,  // ♀ (IBM: Female symbol)
    CP437_MUSICAL_NOTE           = 0x0D,  // ♪ (IBM: Musical note)
    CP437_DOUBLE_NOTE            = 0x0E,  // ♫ (IBM: Double note)
    CP437_SUN                    = 0x0F,  // ☼ (IBM: Sun)
    CP437_RIGHT_ARROW            = 0x10,  // ► (IBM: Right arrow)
    CP437_LEFT_ARROW             = 0x11,  // ◄ (IBM: Left arrow)
    CP437_UP_DOWN_ARROW          = 0x12,  // ↕ (IBM: Up/down arrow)
    CP437_DOUBLE_EXCLAMATION     = 0x13,  // ‼ (IBM: Double exclamation)
    CP437_PARAGRAPH              = 0x14,  // ¶ (IBM: Paragraph)
    CP437_SECTION_SIGN           = 0x15,  // § (IBM: Section sign)
    CP437_RECTANGLE              = 0x16,  // ▬ (IBM: Rectangle)
    CP437_UP_DOWN_ARROW_BASE     = 0x17,  // ↨ (IBM: Up/down arrow with base)
    CP437_UP_ARROW               = 0x18,  // ↑ (IBM: Up arrow)
    CP437_DOWN_ARROW             = 0x19,  // ↓ (IBM: Down arrow)
    CP437_RIGHT_ARROW_IBM        = 0x1A,  // → (IBM: Right arrow)
    CP437_LEFT_ARROW_IBM         = 0x1B,  // ← (IBM: Left arrow)
    CP437_RIGHT_ANGLE            = 0x1C,  // ∟ (IBM: Right angle)
    CP437_LEFT_RIGHT_ARROW       = 0x1D,  // ↔ (IBM: Left-right arrow)
    CP437_UP_TRIANGLE            = 0x1E,  // ▲ (IBM: Up triangle)
    CP437_DOWN_TRIANGLE          = 0x1F,  // ▼ (IBM: Down triangle)

    // =============================================
    // Standard ASCII Printable (0x20-0x7E)
    // =============================================
    CP437_SPACE                  = 0x20,  // (Space)
    CP437_EXCLAMATION            = 0x21,  // !
    // ... (omitted for brevity; add as needed) ...
    CP437_TILDE                  = 0x7E,  // ~

    // =============================================
    // Extended ASCII / IBM Special (0x7F)
    // =============================================
    CP437_HOUSE_SYMBOL           = 0x7F,  // ⌂ (IBM: House symbol)

    // =============================================
    // Extended CP437: Latin Letters with Diacritics (0x80-0x9F)
    // =============================================
    CP437_LATIN_C_CEDILLA        = 0x80,  // Ç
    CP437_LATIN_U_UMLAUT         = 0x81,  // ü
    CP437_LATIN_E_ACUTE          = 0x82,  // é
    CP437_LATIN_A_CIRCUMFLEX     = 0x83,  // â
    CP437_LATIN_A_UMLAUT         = 0x84,  // ä
    CP437_LATIN_A_GRAVE          = 0x85,  // à
    CP437_LATIN_A_RING           = 0x86,  // å
    CP437_LATIN_C_CEDILLA_SMALL  = 0x87,  // ç
    CP437_LATIN_E_CIRCUMFLEX     = 0x88,  // ê
    CP437_LATIN_E_UMLAUT         = 0x89,  // ë
    CP437_LATIN_E_GRAVE          = 0x8A,  // è
    CP437_LATIN_I_UMLAUT         = 0x8B,  // ï
    CP437_LATIN_I_CIRCUMFLEX     = 0x8C,  // î
    CP437_LATIN_I_GRAVE          = 0x8D,  // ì
    CP437_LATIN_A_UMLAUT_CAP     = 0x8E,  // Ä
    CP437_LATIN_A_RING_CAP       = 0x8F,  // Å
    CP437_LATIN_E_ACUTE_CAP      = 0x90,  // É
    CP437_LATIN_AE_SMALL         = 0x91,  // æ
    CP437_LATIN_AE_CAP           = 0x92,  // Æ
    CP437_LATIN_O_CIRCUMFLEX     = 0x93,  // ô
    CP437_LATIN_O_UMLAUT         = 0x94,  // ö
    CP437_LATIN_O_GRAVE          = 0x95,  // ò
    CP437_LATIN_U_CIRCUMFLEX     = 0x96,  // û
    CP437_LATIN_U_GRAVE          = 0x97,  // ù
    CP437_LATIN_Y_UMLAUT         = 0x98,  // ÿ
    CP437_LATIN_O_UMLAUT_CAP     = 0x99,  // Ö
    CP437_LATIN_U_UMLAUT_CAP     = 0x9A,  // Ü
    CP437_CENT_SIGN              = 0x9B,  // ¢
    CP437_POUND_SIGN             = 0x9C,  // £
    CP437_YEN_SIGN               = 0x9D,  // ¥
    CP437_PESETA_SIGN            = 0x9E,  // ₧
    CP437_LATIN_F_HOOK           = 0x9F,  // ƒ

    // =============================================
    // Extended CP437: Symbols and Punctuation (0xA0-0xBF)
    // =============================================
    CP437_LATIN_A_ACUTE          = 0xA0,  // á
    CP437_LATIN_I_ACUTE          = 0xA1,  // í
    CP437_LATIN_O_ACUTE          = 0xA2,  // ó
    CP437_LATIN_U_ACUTE          = 0xA3,  // ú
    CP437_LATIN_N_TILDE          = 0xA4,  // ñ
    CP437_LATIN_N_TILDE_CAP      = 0xA5,  // Ñ
    CP437_FEMININE_ORDINAL       = 0xA6,  // ª
    CP437_MASCULINE_ORDINAL      = 0xA7,  // º
    CP437_INVERTED_QUESTION      = 0xA8,  // ¿
    CP437_REVERSED_NOT_SIGN      = 0xA9,  // ⌐
    CP437_NOT_SIGN               = 0xAA,  // ¬
    CP437_FRACTION_ONE_HALF      = 0xAB,  // ½
    CP437_FRACTION_ONE_QUARTER   = 0xAC,  // ¼
    CP437_INVERTED_EXCLAMATION   = 0xAD,  // ¡
    CP437_LEFT_DOUBLE_ANGLE      = 0xAE,  // «
    CP437_RIGHT_DOUBLE_ANGLE     = 0xAF,  // »
   
    // =============================================
    // Extended CP437: Box Drawing (0xB0-0xDF)
    // =============================================
   CP437_LIGHT_SHADE            = 0xB0,  // ░
    CP437_MEDIUM_SHADE           = 0xB1,  // ▒
    CP437_DARK_SHADE             = 0xB2,  // ▓
    CP437_BOX_VERTICAL           = 0xB3,  // │
    CP437_BOX_VERTICAL_LEFT      = 0xB4,  // ┤
    CP437_BOX_V_SINGLE_LEFT_DBL  = 0xB5,  // ╡
    CP437_BOX_V_DOUBLE_LEFT_SGL  = 0xB6,  // ╢
    CP437_BOX_DOWN_DBL_LEFT_SGL  = 0xB7,  // ╖
    CP437_BOX_DOWN_SGL_LEFT_DBL  = 0xB8,  // ╕
    CP437_BOX_DOUBLE_V_LEFT      = 0xB9,  // ╣
    CP437_BOX_DOUBLE_VERTICAL    = 0xBA,  // ║
    CP437_BOX_DOUBLE_DOWN_LEFT   = 0xBB,  // ╗
    CP437_BOX_DOUBLE_UP_LEFT     = 0xBC,  // ╝
    CP437_BOX_UP_DBL_LEFT_SGL    = 0xBD,  // ╜
    CP437_BOX_UP_SGL_LEFT_DBL    = 0xBE,  // ╛
    CP437_BOX_DOWN_LEFT          = 0xBF,  // ┐

    
    CP437_BOX_UP_RIGHT           = 0xC0,  // └
    CP437_BOX_UP_HORIZONTAL      = 0xC1,  // ┴
    CP437_BOX_DOWN_HORIZONTAL    = 0xC2,  // ┬
    CP437_BOX_VERTICAL_RIGHT     = 0xC3,  // ├
    CP437_BOX_HORIZONTAL         = 0xC4,  // ─
    CP437_BOX_VERTICAL_HORIZONTAL= 0xC5,  // ┼
    CP437_BOX_V_SINGLE_RIGHT_DBL = 0xC6,  // ╞
    CP437_BOX_V_DOUBLE_RIGHT_SGL = 0xC7,  // ╟
    CP437_BOX_DOUBLE_UP_RIGHT    = 0xC8,  // ╚
    CP437_BOX_DOUBLE_DOWN_RIGHT  = 0xC9,  // ╔
    CP437_BOX_DOUBLE_UP_HORIZ    = 0xCA,  // ╩
    CP437_BOX_DOUBLE_DOWN_HORIZ  = 0xCB,  // ╦
    CP437_BOX_DOUBLE_V_RIGHT     = 0xCC,  // ╠
    CP437_BOX_DOUBLE_HORIZONTAL  = 0xCD,  // ═
    CP437_BOX_DOUBLE_V_HORIZ     = 0xCE,  // ╬
    CP437_BOX_UP_SGL_HORIZ_DBL   = 0xCF,  // ╧
    CP437_BOX_UP_DBL_HORIZ_SGL   = 0xD0,  // ╨
    CP437_BOX_DOWN_SGL_HORIZ_DBL = 0xD1,  // ╤
    CP437_BOX_DOWN_DBL_HORIZ_SGL = 0xD2,  // ╥
    CP437_BOX_UP_DBL_RIGHT_SGL   = 0xD3,  // ╙
    CP437_BOX_UP_SGL_RIGHT_DBL   = 0xD4,  // ╘
    CP437_BOX_DOWN_SGL_RIGHT_DBL = 0xD5,  // ╒
    CP437_BOX_DOWN_DBL_RIGHT_SGL = 0xD6,  // ╓
    CP437_BOX_DOUBLE_V_HORIZ_ALT = 0xD7,  // ╫
    CP437_BOX_V_DBL_HORIZ_SGL    = 0xD8,  // ╪
    CP437_BOX_UP_LEFT            = 0xD9,  // ┘
    CP437_BOX_DOWN_RIGHT         = 0xDA,  // ┌
    CP437_FULL_BLOCK             = 0xDB,  // █
    CP437_LOWER_HALF_BLOCK       = 0xDC,  // ▄
    CP437_LEFT_HALF_BLOCK        = 0xDD,  // ▌
    CP437_RIGHT_HALF_BLOCK       = 0xDE,  // ▐
    CP437_UPPER_HALF_BLOCK       = 0xDF,  // ▀

    // =============================================
    // Extended CP437: Greek and Math Symbols (0xE0-0xFF)
    // =============================================
    CP437_GREEK_ALPHA            = 0xE0,  // α
    CP437_LATIN_SHARP_S          = 0xE1,  // ß
    CP437_GREEK_GAMMA            = 0xE2,  // Γ
    CP437_GREEK_PI               = 0xE3,  // π
    CP437_GREEK_SIGMA            = 0xE4,  // Σ
    CP437_GREEK_SIGMA_SMALL      = 0xE5,  // σ
    CP437_MICRO_SIGN             = 0xE6,  // µ
    CP437_GREEK_TAU              = 0xE7,  // τ
    CP437_GREEK_PHI_CAP          = 0xE8,  // Φ
    CP437_GREEK_THETA_CAP        = 0xE9,  // Θ
    CP437_GREEK_OMEGA_CAP        = 0xEA,  // Ω
    CP437_GREEK_DELTA            = 0xEB,  // δ
    CP437_INFINITY               = 0xEC,  // ∞
    CP437_GREEK_PHI              = 0xED,  // φ
    CP437_GREEK_EPSILON          = 0xEE,  // ε
    CP437_INTERSECTION           = 0xEF,  // ∩
    CP437_IDENTICAL_TO           = 0xF0,  // ≡
    CP437_PLUS_MINUS             = 0xF1,  // ±
    CP437_GREATER_THAN_OR_EQUAL  = 0xF2,  // ≥
    CP437_LESS_THAN_OR_EQUAL     = 0xF3,  // ≤
    CP437_TOP_HALF_INTEGRAL      = 0xF4,  // ⌠
    CP437_BOTTOM_HALF_INTEGRAL   = 0xF5,  // ⌡
    CP437_DIVISION_SIGN          = 0xF6,  // ÷
    CP437_ALMOST_EQUAL           = 0xF7,  // ≈
    CP437_DEGREE_SIGN            = 0xF8,  // °
    CP437_BULLET_OPERATOR        = 0xF9,  // ∙
    CP437_MIDDLE_DOT             = 0xFA,  // ·
    CP437_SQUARE_ROOT            = 0xFB,  // √
    CP437_SUPERSCRIPT_N           = 0xFC,  // ⁿ
    CP437_SUPERSCRIPT_TWO        = 0xFD,  // ²
    CP437_BLACK_SQUARE           = 0xFE,  // ■
    CP437_NO_BREAK_SPACE         = 0xFF   // (Non-breaking space)
} CP437_Char;

#endif // CP437_CONSTANTS_H
