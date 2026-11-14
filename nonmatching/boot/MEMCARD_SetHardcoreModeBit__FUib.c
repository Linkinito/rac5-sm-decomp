/* USA SCUS_976.15 boot: MEMCARD_SetHardcoreModeBit__FUib, 0x01E62848.
 * Privately reconstructed from the owner's retail program; no imported source.
 * Observed body: 48 bytes; extent remains provisional, not an acceptance claim.
 * Measured 14 November 2026 with rebuilt GNU EE and SDK tools, -O2 -G0,
 * GNU assembler and independent SN link at the retail address:
 * 44 compiled bytes; 23 equal byte positions in the 44-byte compared prefix;
 * first differing byte +0x02. Pinned-route, gnu-ee-trial and sdk991111 agree
 * on this source's linked text. The pinned-route tools are rebuilt, not qualified.
 * Sets bit 0 at memcard_region + slot * 20 + 0x7B.
 * The integer temporary retains the conditional selection; the compiler removes
 * a redundant byte mask and chooses different registers. Three scalar forms
 * and a byte-bitfield view did not reproduce the retail sequence.
 * The boolean argument uses the int convention of the existing callers.
 * Reopen with evidence for the original type/lifetime representation or a
 * measured compiler-lowering explanation that preserves accepted controls.
 * Not exact; no complete-program gate, promotion or C matching credit.
 */
extern unsigned char memcard_region[];

void MEMCARD_SetHardcoreModeBit__FUib(unsigned int slot, int enabled) {
    unsigned char *p = &memcard_region[slot * 20u];
    unsigned char flags = p[0x7B];
    flags &= ~1u;
    {
        unsigned int value = flags;
        if (enabled)
            value |= 1;
        p[0x7B] = value;
    }
}
