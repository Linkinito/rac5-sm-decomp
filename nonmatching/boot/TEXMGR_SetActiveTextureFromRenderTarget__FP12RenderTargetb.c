/* USA SCUS_976.15 boot, TEXMGR_SetActiveTextureFromRenderTarget__FP12RenderTargetb
 * at 0x01E7BFC0. Observed body: 712 bytes (jr ra plus delay slot end the
 * 712-byte interval; no padding word). Reconstructed from the retail program,
 * using the original symbols. Measured 28 November 2026 under the default
 * pinned route, -O2 -G0, default GNU assembler: 736 bytes, first difference
 * +0x8, not exact (gnu-ee-trial: 748 bytes; divpad routes: 736, like the
 * pinned route). No complete-program gate, promotion or C matching credit.
 *
 * Opens a new packet (cnt tag copy and unpack header), computes the TEX1 K
 * value as float_to_k(-log2(10000)), and sends five GS register writes in a
 * direct-mode bracket: TEXFLUSH, TEX1 (MMAG 1, MMIN 5, K), TEX0 built from the
 * target (base pointer = VRAM address >> 8, buffer width in 64-pixel units with
 * a minimum of 1, pixel format from +0x10, log2 width and height rounded up,
 * CLD 2, TCC from the flag), and the two MIPTBP addresses with their data
 * words left as they are. It also clears an unused 24-byte local. Source
 * forms that mattered: TEX0 as one nested shift-or expression of signed ints
 * (no zero extensions, as in the retail), the five address words written
 * before the size loops.
 * Blockers: (1) the two 64-bit TEX1/TEX0 clear masks are dli constants that
 * the pinned GNU assembler expands to seven instructions each; the retail uses
 * the four-instruction lui/ori/dsll/ori form of SN Ps2EeAs (24 of the 24
 * extra bytes); Ps2EeAs was not available for this measurement. (2) register
 * allocation: the retail keeps the target pointer, the base pointer and the
 * display list in s0, s1 and s5 and the QWdata pointer in fp (nine
 * callee-saved registers); the compiler reuses s0 for the height and needs
 * eight. Reopen under the ps2eeas assembler, then work on the load order of
 * width, height and pixel format that decides the register conflicts.
 * Also measured 28 November 2026 with the SN Ps2EeAs assembler route
 * (as=ps2eeas): the two 64-bit clear masks expand to retail's four-instruction
 * form and the default route then emits the retail 712 bytes (174 equal);
 * gnu-ee-trial gives 728. What remains is the callee-saved register
 * allocation described above.
 */
typedef struct RenderTarget {
    unsigned int field00;
    unsigned int field04;
    unsigned short width;
    unsigned short height;
    unsigned short field0C;
    unsigned short field0E;
    unsigned int field10;
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

typedef struct {
    unsigned long data;
    unsigned long addr;
} QWdata;

typedef struct {
    unsigned long lcm : 1;
    unsigned long pad01 : 1;
    unsigned long mxl : 3;
    unsigned long mmag : 1;
    unsigned long mmin : 3;
    unsigned long mtba : 1;
    unsigned long pad0A : 9;
    unsigned long l : 2;
    unsigned long pad15 : 11;
    unsigned long k : 12;
    unsigned long pad2C : 20;
} GsTex1;

typedef struct {
    unsigned long tbp0 : 14;
    unsigned long tbw : 6;
    unsigned long psm : 6;
    unsigned long tw : 4;
    unsigned long th : 4;
    unsigned long tcc : 1;
    unsigned long tfx : 2;
    unsigned long cbp : 14;
    unsigned long cpsm : 4;
    unsigned long csm : 1;
    unsigned long csa : 5;
    unsigned long cld : 3;
} GsTex0;

extern tDbDlist db_dlist;
extern char g_cntTag[];
extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);
extern float logf(float);
extern int float_to_k__Ff(float);
extern void dlib_displist_direct_begin__FP13DLIB_DISPLIST(DLIB_DISPLIST *);
extern void dlib_displist_add_data_block__FP13DLIB_DISPLISTP6QWdatai(DLIB_DISPLIST *, QWdata *, int);
extern void dlib_displist_direct_end__FP13DLIB_DISPLIST(DLIB_DISPLIST *);

void TEXMGR_SetActiveTextureFromRenderTarget__FP12RenderTargetb(RenderTarget *target, unsigned char tcc) {
    QWdata regs[5];
    unsigned long clear[3];
    DLIB_DISPLIST *dl = db_dlist.pDlist;
    int tbp = target->field00 >> 8;
    unsigned int *p;
    int k;
    int width;
    int height;
    int psm;
    int tbw;
    int tw;
    int th;
    QWdata *q;
    GsTex1 *tex1;

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

    k = float_to_k__Ff(-(logf(10000.0f) / 0.6931472f));
    width = target->width;
    height = target->height;
    psm = target->field10;
    q = regs;
    memset(clear, 0, sizeof(clear));
    tbw = width / 64;
    regs[1].addr = 0x14;
    regs[2].addr = 6;
    regs[3].addr = 0x34;
    regs[4].addr = 0x36;
    regs[0].addr = 0x3F;
    if (tbw == 0) {
        tbw = 1;
    }
    for (th = 0; (1 << th) < height; th++) {
    }
    for (tw = 0; (1 << tw) < width; tw++) {
    }
    q[0].data = 1;
    tex1 = (GsTex1 *)&q[1].data;
    tex1->lcm = 0;
    tex1->mxl = 0;
    tex1->mmag = 1;
    tex1->mmin = 5;
    tex1->mtba = 0;
    tex1->l = 0;
    tex1->k = k;
    q[2].data = ((((((unsigned long)th | 0x10 | (1UL << 32)) << 4 | tw) << 6 | psm) << 6 | tbw) << 14) | tbp;
    ((GsTex0 *)&regs[2].data)->tcc = tcc;
    dlib_displist_direct_begin__FP13DLIB_DISPLIST(dl);
    dlib_displist_add_data_block__FP13DLIB_DISPLISTP6QWdatai(dl, regs, 5);
    dlib_displist_direct_end__FP13DLIB_DISPLIST(dl);
}
