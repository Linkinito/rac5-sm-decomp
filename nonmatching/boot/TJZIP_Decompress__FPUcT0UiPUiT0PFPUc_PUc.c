/* 0x01E8D940 -- Decompress a TJZIP stream: raw-data blocks and dictionary codes are
 * copied to the destination while updating a CRC, the read callback is installed
 * for the parsers, and the thread yields with DelayThread(0x320) when about
 * 0x168000 timer ticks pass. Returns the decompressed length, or 0 when the
 * source and destination cursors cross. Observed body 592 bytes (0x01E8D940-
 * 0x01E8DB90, no alignment word). Measured 28 November 2026: FULL_EXACT under the
 * default pinned route and gnu-ee-trial. */

typedef unsigned char u8;

extern u8 *(*g_pCallback)(u8 *lim);
extern u8 *g_pCallbackAt;
extern unsigned g_signatureTable[];

extern unsigned long long GetTimerSystemTime(void);
extern void DelayThread(unsigned int micros);
extern unsigned TJZIP_ParseDictionaryCode__FPPUcT0PUi(u8 **src, u8 **dst, unsigned *crc);
extern void TJZIP_ParseRawDataBlock__FPPUcT0PUi(u8 **src, u8 **dst, unsigned *crc);
extern int abs(int);

unsigned TJZIP_Decompress__FPUcT0UiPUiT0PFPUc_PUc(u8 *src, u8 *dst, unsigned srcLen,
    unsigned *crc, u8 *callbackAt, u8 *(*callback)(u8 *))
{
    u8 *srcStart;
    u8 *dstStart;
    int backwards;
    long t;
    unsigned n;
    unsigned i;
    u8 *q;

    srcStart = src;
    dstStart = dst;
    backwards = src < dst;
    *crc = 0;
    g_pCallbackAt = callbackAt;
    g_pCallback = callback;
    TJZIP_ParseRawDataBlock__FPPUcT0PUi(&src, &dst, crc);
    t = GetTimerSystemTime();
    while ((unsigned)abs(src - srcStart) < srcLen) {
        if ((long)GetTimerSystemTime() > t + 0x168000) {
            DelayThread(0x320);
            t = GetTimerSystemTime();
        }
        n = TJZIP_ParseDictionaryCode__FPPUcT0PUi(&src, &dst, crc);
        if (n != 3 && (unsigned)abs(src - srcStart) < srcLen) {
            if (n != 0) {
                for (i = 0; i < n; i++) {
                    *dst = *src;
                    *crc = (*crc >> 8) ^ g_signatureTable[*src ^ (*crc & 0xFF)];
                    src++;
                    q = src;
                    if (g_pCallback != 0) {
                        if (g_pCallbackAt != 0 && q >= g_pCallbackAt - 0x20) {
                            g_pCallbackAt = g_pCallback(g_pCallbackAt);
                        }
                    }
                    dst++;
                }
            } else {
                TJZIP_ParseRawDataBlock__FPPUcT0PUi(&src, &dst, crc);
            }
        }
        if (backwards != (src < dst)) {
            return 0;
        }
    }
    *crc = ~*crc;
    return abs(dst - dstStart);
}
