/* USA SCUS_976.15 boot: MEMCARD_SetDanSkinUnlocked__FUi, 0x01E627D0.
 * Privately reconstructed from the owner's retail program; no imported source.
 * Observed body: 36 bytes; extent remains provisional, not an acceptance claim.
 * Measured 14 November 2026 with rebuilt GNU EE and SDK tools, -O2 -G0,
 * GNU assembler and independent SN link at the retail address:
 * 32 compiled bytes; 14 equal byte positions in the 32-byte compared prefix;
 * first differing byte +0x02. Pinned-route, gnu-ee-trial and sdk991111 agree
 * on this source's linked text. The pinned-route tools are rebuilt, not qualified.
 * Sets bit 1 at memcard_region + 0x64 from a nonzero unsigned argument.
 * The integer temporary retains the conditional selection; the compiler removes
 * a redundant byte mask and chooses different registers. Three scalar forms
 * and a byte-bitfield view did not reproduce the retail sequence.
 * Reopen with evidence for the original type/lifetime representation or a
 * measured compiler-lowering explanation that preserves accepted controls.
 * Not exact; no complete-program gate, promotion or C matching credit.
 */
extern unsigned char memcard_region[];

void MEMCARD_SetDanSkinUnlocked__FUi(unsigned int enabled) {
    unsigned char flags = memcard_region[0x64];
    flags &= ~2u;
    {
        unsigned int value = flags;
        if (enabled)
            value |= 2;
        memcard_region[0x64] = value;
    }
}
