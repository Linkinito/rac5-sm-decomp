/* USA SCUS_976.15 boot, BLOOM_Downsample__FP12RenderTargetT0 at 0x01E7D6D8.
 * Observed body: 644 bytes (jr ra plus delay slot), followed by one zero
 * padding word inside the 648-byte interval. Reconstructed from the retail
 * program, using the original symbols. Measured 28 November 2026 under
 * gnu-ee-trial, -O2 -G0, default GNU assembler: retail size, 635 of 644 body
 * bytes equal (cmp2 resolved against retail symbol addresses), first
 * difference +0xDC; the pinned route also differs in the prologue order
 * (617 of 644), the divpad routes equal gnu-ee-trial. Not exact; no
 * complete-program gate, promotion or C matching credit.
 *
 * Renders the first target into the second at half width: selects the first
 * target as the source, sets blend state 0xA8 with fix 0x40, opens a new
 * packet (cnt tag copy and unpack header), makes the second target current,
 * enables alpha test and blending, clears it column by column with a flat
 * black rectangle, then draws one textured 32-pixel column per 64-pixel
 * source column and ends with a TEXFLUSH write. Source forms that mattered:
 * the packet header words written in ascending order; the clearing loop as
 * an explicit guarded do-while (a for loop lets the first scheduling pass
 * hoist the counter reset above the calls, adds a delay-slot nop and a loop
 * alignment nop: 652 bytes).
 * Remaining difference: three words in the argument set-up of
 * SYSTEM_SetAlphaTestMode (1, 0, 1): retail loads the second and third
 * arguments first and fills the delay slot with the first, the compiler
 * loads the first argument first and fills the slot with the third.
 * Argument types (int, unsigned char, char, unprototyped) do not change it.
 * Reopen with a source form or route that orders the constant argument
 * loads of that call as the retail does.
 */
typedef struct RenderTarget {
    unsigned int field00;
    unsigned int field04;
    unsigned short width;
    unsigned short height;
} RenderTarget;

typedef struct {
    char pad0[4];
    unsigned int *cur;
    unsigned int *begin;
} DLIB_DISPLIST;

typedef struct {
    char pad[0x54];
    DLIB_DISPLIST *pDlist;
} tDbDlist;

extern tDbDlist db_dlist;
extern char g_cntTag[];
extern float halfOffset2;
extern void *memcpy(void *, const void *, unsigned int);
extern void VRAM_RenderFrom__FP12RenderTargetb(RenderTarget *, int);
extern void VRAM_RenderTo__FP12RenderTargetUi(RenderTarget *, unsigned int);
extern void dlib_displist_set_blend_state__FP13DLIB_DISPLISTUl(DLIB_DISPLIST *, unsigned long);
extern void SYSTEM_SetAlphaTestMode__FbUcUi(int, int, unsigned int);
extern void SYSTEM_SetBlendEnabled__Fb(int);
extern void UTIL_DrawFlatShadedRect__FssssUiUi(short, short, short, short, unsigned int, unsigned int);
extern void UTIL_DrawTexturedRect__FssssffffUiUi(short, short, short, short, float, float, float, float, unsigned int, unsigned int);
extern void dlib_displist_direct_begin__FP13DLIB_DISPLIST(DLIB_DISPLIST *);
extern void dlib_displist_add_data_ul64__FP13DLIB_DISPLISTUlUl(DLIB_DISPLIST *, unsigned long, unsigned long);
extern void dlib_displist_direct_end__FP13DLIB_DISPLIST(DLIB_DISPLIST *);

void BLOOM_Downsample__FP12RenderTargetT0(RenderTarget *src, RenderTarget *dst) {
    DLIB_DISPLIST *dl = db_dlist.pDlist;
    unsigned int n = src->width >> 6;
    unsigned int i;
    unsigned int j;
    unsigned int *p;

    VRAM_RenderFrom__FP12RenderTargetb(src, 0);
    dlib_displist_set_blend_state__FP13DLIB_DISPLISTUl(dl, (0x40UL << 32) | 0xA8);
    memcpy(dl->begin, g_cntTag, 16);
    p = dl->cur;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0x13000000;
    dl->cur += 4;
    *(unsigned short *)dl->begin = 1;
    dl->begin = dl->cur;
    dl->cur += 4;
    VRAM_RenderTo__FP12RenderTargetUi(dst, 0);
    SYSTEM_SetAlphaTestMode__FbUcUi(1, 0, 1);
    SYSTEM_SetBlendEnabled__Fb(1);
    if (dst->width >> 6) {
        j = 0;
        do {
            UTIL_DrawFlatShadedRect__FssssUiUi(0, 0, 0x40, dst->height, 0xFF000000, 0x00FFFFFF);
        } while (++j < (unsigned int)(dst->width >> 6));
    }
    for (i = 0; i < n; i++) {
        UTIL_DrawTexturedRect__FssssffffUiUi(i * 32, 0, 0x20, dst->height,
            halfOffset2 + i * 64, halfOffset2 + 0.0f, halfOffset2 + 64.0f + i * 64, halfOffset2 + src->height,
            0xFFFFFFFF, 0x00FFFFFF);
    }
    dlib_displist_direct_begin__FP13DLIB_DISPLIST(dl);
    dlib_displist_add_data_ul64__FP13DLIB_DISPLISTUlUl(dl, 1, 0x3F);
    dlib_displist_direct_end__FP13DLIB_DISPLIST(dl);
}
