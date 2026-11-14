/* USA SCUS_976.15 boot: MEMCARD_Saving__Fv, 0x01E62998.
 * Privately reconstructed from the owner's retail program; no imported source.
 * Observed body: 52 bytes; extent remains provisional, not an acceptance claim.
 * Measured 14 November 2026 with rebuilt GNU EE and SDK tools, -O2 -G0,
 * GNU assembler and independent SN link at the retail address:
 * 48 compiled bytes; 38 equal byte positions in the 48-byte compared prefix;
 * first differing byte +0x14. Pinned-route, gnu-ee-trial and sdk991111 agree
 * on this source's linked text. The pinned-route tools are rebuilt, not qualified.
 * Returns 1 for states 11 through 17 when the secondary state is 2.
 * States are unsigned bytes at memcard_region + 0x1A and +0x1B.
 * A shared return omits a value transfer kept by retail. A nested comparison
 * is identical; an early return reaches 52 bytes but changes the return paths.
 * Reopen with evidence for the original type/lifetime representation or a
 * measured compiler-lowering explanation that preserves accepted controls.
 * Not exact; no complete-program gate, promotion or C matching credit.
 */
extern unsigned char memcard_region[];

int MEMCARD_Saving__Fv(void) {
    int result = 0;
    if ((unsigned int)(memcard_region[0x1A] - 11) < 7)
        result = memcard_region[0x1B] == 2;
    return result;
}
