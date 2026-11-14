/* USA SCUS_976.15 boot: MEMCARD_SetHeaderDetail__FUiUiUiUiUi, 0x01E628D0.
 * Privately reconstructed from the owner's retail program; no imported source.
 * Observed body: 52 bytes; extent remains provisional, not an acceptance claim.
 * Measured 14 November 2026 with rebuilt GNU EE and SDK tools, -O2 -G0,
 * GNU assembler and independent SN link at the retail address:
 * 40 compiled bytes; 22 equal byte positions in the 40-byte compared prefix;
 * first differing byte +0x11. Pinned-route, gnu-ee-trial and sdk991111 agree
 * on this source's linked text. The pinned-route tools are rebuilt, not qualified.
 * Writes header fields at offsets 0x68, 0x6C, 0x70 and 0x74, stride 20.
 * The second payload is narrowed to one byte; the other payloads are words.
 * The linked base is four-byte aligned and the slot stride preserves alignment.
 * Byte offsets and a typed field view compile identically: pointer copies
 * disappear and the last payload store moves before the other stores.
 * Reopen with evidence for the original type/lifetime representation or a
 * measured compiler-lowering explanation that preserves accepted controls.
 * Not exact; no complete-program gate, promotion or C matching credit.
 */
extern unsigned char memcard_region[];

void MEMCARD_SetHeaderDetail__FUiUiUiUiUi(unsigned int slot, unsigned int a,
                                      unsigned int b, unsigned int c, unsigned int d) {
    unsigned char *p = &memcard_region[slot * 20u];
    *(unsigned int *)(p + 0x68) = a;
    p[0x6C] = b;
    *(unsigned int *)(p + 0x70) = c;
    *(unsigned int *)(p + 0x74) = d;
}
