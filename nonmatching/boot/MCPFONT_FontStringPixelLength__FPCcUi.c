/* USA SCUS_976.15 boot: MCPFONT_FontStringPixelLength__FPCcUi,
 * 0x01E679B8..0x01E67A88, observed 208-byte body, no trailing alignment word.
 * Reconstructed from the owner's retail body, helper and accepted wrapper.
 * Measured 16 November 2026 with -O2 -G0 and independent SN links:
 * rebuilt default GNU and gnu-ee-trial produce 208 bytes, 194 equal byte
 * positions, first difference at +0x2D. Not exact; no complete-program gate,
 * qualified match, acceptance or promotion.
 * The 14 November direct-comparison form emitted 216 bytes, 63 equal positions
 * and first difference +0x1C. Returning the control classification from an
 * ordinary inline equality helper removes its loop-held comparison constant
 * and extra saved register. This is an authored parser helper, not a claim
 * to have recovered a separately named retail function. No XOR operation,
 * assembly or fixed-register constraint is used to request an instruction.
 * The four-byte glyph return and byte-2 width are proven from both sides;
 * direct member consumption avoids an extra aggregate copy.
 * Byte 0x90 skips itself and the following byte; a nonzero font page consumes
 * one additional byte. Original reads past the supplied count are preserved.
 * The total is reduced to 16 bits after adding each glyph's width plus one.
 * Remaining differences exchange the index and width register assignments.
 * Reopen with new allocation/lifetime evidence; the measured declaration,
 * initialization and loop rewrites do not explain that exchange.
 */
typedef struct {
    unsigned char field0;
    unsigned char field1;
    unsigned char width;
    unsigned char field3;
} MCPFONT_CharMetrics;
typedef char MCPFONT_CharMetrics_size_check[sizeof(MCPFONT_CharMetrics) == 4 ? 1 : -1];

extern int MCPFONT_GetFontPage__FUc(unsigned char character);
extern MCPFONT_CharMetrics func_1E678A0(unsigned char character, unsigned char page);

static inline int mcpfont_is_control_byte(unsigned char character) {
    return character == 0x90;
}

int MCPFONT_FontStringPixelLength__FPCcUi(const char *text, unsigned int length) {
    unsigned int i;
    unsigned int width;

    if (text == 0)
        return 0;
    for (i = 0, width = 0; i < length; ++i) {
        unsigned char character = text[i];
        int page;

        if (mcpfont_is_control_byte(character)) {
            ++i;
            continue;
        }
        ++width;
        page = MCPFONT_GetFontPage__FUc(character);
        if (page != 0)
            ++i;
        width = (unsigned short)(width + func_1E678A0((unsigned char)text[i], (unsigned char)page).width);
    }
    return width;
}
