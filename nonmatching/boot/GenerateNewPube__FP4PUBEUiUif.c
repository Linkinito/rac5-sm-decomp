/* GenerateNewPube__FP4PUBEUiUif - 0x01E80478, observed body 692 bytes (jr ra + delay slot)
   followed by one zero alignment word (696-byte next-label interval).
   Builds a random strand: clamps the length to 35 and rounds it up to the next multiple of
   four, stores it and the float argument, picks a random start point and heading, then walks
   the points with step 3, turning sharply every `kink` points and drifting slightly otherwise,
   truncating each coordinate through short. Unused tail points are parked at (255, 208, 0).
   Measured 28 November 2026 (cc1 flags -O2 -G0 -mno-check-zero-division):
     gnu-ee-trial cc1 + pinned as: 708/692 bytes, 414 equal, first difference at byte 288.
     gnu-ee-trial cc1 + r5900-mfc1 as (a level-only assembler today): 700/692, 416 equal,
     first difference at byte 288.
   Remaining: (1) the pinned as inserts a load-delay nop after each mfc1 that the retail omits
   (r5900-mfc1 removes them); (2) the three loop-invariant constant loads before the first loop
   are hoisted in the order 3.0, 628, 400 where the retail has 400, 628, 3.0; (3) with that
   loop label aligned, the assembler flushes the preceding mtc1 delay and pads with two nops
   before the loop head, which the retail does not have. Register allocation, branch layout and
   the second (aligned) loop already agree.
   Reopen when a boot assembler without the mfc1 delay is qualified and a hypothesis explains
   the movable order (loop.c) or the unaligned first loop. */
typedef struct { float x, y, z; } VEC3;
typedef struct { unsigned int count; float f4; char pad[8]; VEC3 pts[40]; } PUBE;
extern int rand(void);
extern float cosf(float);
extern float sinf(float);

void GenerateNewPube__FP4PUBEUiUif(PUBE *p, unsigned int kink, unsigned int len, float f) {
    
    unsigned int x, y;
    unsigned int i;
    unsigned int j;
    float angle;

    if (len > 35) len = 35;
    len = len + 4 - len % 4;
    p->count = len;
    p->f4 = f;
    x = rand() % 480 + 16;
    y = rand() % 384 + 16;
    angle = (float)(rand() % 628) / 100.0f;
    p->pts[0].x = x;
    p->pts[0].y = y;
    for (i = 1; i < len; i++) {
        if (i % kink == 0) {
            angle += (float)(rand() % 628) / 400.0f;
        } else {
            angle += (float)(rand() % 628) / 10000.0f - 0.0628f;
        }
        p->pts[i].x = (short)(p->pts[i - 1].x + cosf(angle) * 3.0f);
        p->pts[i].y = (short)(p->pts[i - 1].y + sinf(angle) * 3.0f);
    }
    for (j = len; j < 40; j++) {
        p->pts[j].x = 255.0f;
        p->pts[j].y = 208.0f;
        p->pts[j].z = 0.0f;
    }
}
