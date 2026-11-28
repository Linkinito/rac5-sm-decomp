/* JigglePube__FP4PUBE - 0x01E80730, observed body 636 bytes (jr ra + delay slot)
   followed by one zero alignment word (640-byte next-label interval).
   Draws a common random drift for x and y, then for each point adds the drift plus a small
   per-point random jitter, truncated through short; strands under twelve points jitter by
   [-0.5, 1.5), longer ones by [-0.25, 0.75). Both random draws precede the x store.
   Measured 28 November 2026 (cc1 flags -O2 -G0 -mno-check-zero-division):
     gnu-ee-trial cc1 + pinned as: 652/636 bytes, 320 equal, first difference at byte 148.
     gnu-ee-trial cc1 + r5900-mfc1 as (a level-only assembler today): 636/636, 600 equal,
     first difference at byte 161.
   Remaining under r5900-mfc1: register allocation only - in each loop the retail gives the
   counter s2 and the hoisted 1000 s1 (here swapped), and keeps the first jitter value in f20
   from its conversion onwards (here it lands in f20 only after the subtraction), which also
   renames the temporaries of the cvt.w.s/mfc1 pairs. The pinned as adds one nop per mfc1.
   Reopen when a boot assembler without the mfc1 delay is qualified and a new source
   hypothesis changes the allocation of the jitter pseudo and loop counter. */
typedef struct { float x, y, z; } VEC3;
typedef struct { unsigned int count; float f4; char pad[8]; VEC3 pts[40]; } PUBE;
extern int rand(void);

void JigglePube__FP4PUBE(PUBE *p) {
    float dx, dy;
    float jx, jy;
    unsigned int i;

    dx = (float)((rand() >> 4) % 1000) / 500.0f - 0.9f;
    dy = (float)((rand() >> 4) % 1000) / 500.0f - 0.9f;
    if (p->count < 12) {
        for (i = 0; i < p->count; i++) {
            jx = (float)((rand() >> 4) % 1000) / 500.0f - 0.5f;
            jy = (float)((rand() >> 4) % 1000) / 500.0f - 0.5f;
            p->pts[i].x += (short)(dx + jx);
            p->pts[i].y += (short)(dy + jy);
        }
    } else {
        for (i = 0; i < p->count; i++) {
            jx = (float)((rand() >> 4) % 1000) / 1000.0f - 0.25f;
            jy = (float)((rand() >> 4) % 1000) / 1000.0f - 0.25f;
            p->pts[i].x += (short)(dx + jx);
            p->pts[i].y += (short)(dy + jy);
        }
    }
}
