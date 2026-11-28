/* USA SCUS_976.15 boot: moduleThreadContainer__FPv, 0x01E8F700.
 * Observed body: 216 bytes (0x01E8F700..0x01E8F7D7). The 592-byte next-label
 * interval also holds the unnamed static helpers at 0x01E8F7D8 (module system
 * init), 0x01E8F838 (free slot search), 0x01E8F878 (slot reset) and 0x01E8F918.
 * Measured 28 November 2026: all 216 body bytes are exact after relocation at
 * the retail address under the rebuilt default GNU route and gnu-ee-trial, with
 * -O2 -G0 and the default GNU assembler. This is an unaccepted witness:
 * complete-program integration, boundaries, types and qualification require
 * separate review. No promotion or matching credit is claimed here.
 * Reconstructed from the retail boot. Thread entry started by
 * MODULE_StartModule: it exits if the slot is null, not in use or has no entry
 * point (clearing the thread id in that last case), splits the copied argument
 * block into an argv array of up to 16 NUL-terminated strings, calls the entry
 * point stored at offset 0xC with (argc, argv), then marks the slot thread id -1
 * and exits through ExitDeleteThread.
 * The retail code never initialises argc (the counter register is used as
 * found); this original behaviour is preserved by leaving argc uninitialised.
 * The end pointer must be formed from the already computed start pointer.
 */
typedef struct {
    void *heap;
    int dll;
    int active : 1;
    int padC;
    int thread;
    int argLen;
    char args[0x418 - 0x18];
} tMODULE;

extern unsigned int strlen(const char *s);
extern void ExitDeleteThread(void);

void moduleThreadContainer__FPv(void *arg) {
    tMODULE *m = (tMODULE *)arg;
    char *argv[16];
    int argc;
    char *p;
    char *end;

    if (m == 0) {
        ExitDeleteThread();
    }
    if (!m->active) {
        ExitDeleteThread();
    }
    if (m->padC == 0) {
        m->thread = -1;
        ExitDeleteThread();
    }
    p = m->args;
    end = p + m->argLen;
    for (; p < end; p += strlen(p) + 1) {
        argv[argc++] = p;
    }
    ((int (*)(int, char **))m->padC)(argc, argv);
    m->thread = -1;
    ExitDeleteThread();
}
