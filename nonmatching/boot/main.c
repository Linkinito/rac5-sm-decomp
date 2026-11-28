/* USA SCUS_976.15 boot: main at 0x01E65C80.
 * Reconstructed from the retail boot; observed body is 452 bytes (through the
 * return and its delay slot), followed by one zero alignment word at
 * 0x01E65E44. The 664-byte next-label interval also covers two unlabeled
 * functions, at 0x01E65E48 (subsystem initialisation, returns nonzero on
 * success; called here with argc and argv) and 0x01E65EF0 (deinitialisation,
 * ends with stopAndUnloadModules__Fv); they are not part of this body.
 * Measured 28 November 2026: gnu-ee-trial, -O2 -G0, default GNU assembler,
 * reproduces all 452 bytes in an isolated object compare with relocations
 * resolved against retail symbols (FULL_EXACT). The default pinned route
 * differs in prologue save order and the epilogue return-value placement.
 * The program initialises the application and IOP I/O, raises the main
 * thread priority, draws the CD-loading texture for two frames, sets up the
 * buffer cache and memory card, then runs the MCP and memory card update loop
 * while g_MainThreadOn is set. Either early failure shuts I/O down and
 * returns 0. The callee names at 0x01E65E48 and 0x01E65EF0, and the memory
 * card command object at 0x01F5F700, are address placeholders, not recovered
 * symbols. No complete-program gate, acceptance or matching credit claimed.
 */
typedef struct tMEMCARD_CMD tMEMCARD_CMD;

extern int APP_Init__Fv(void);
extern void higIoInit__Fv(void);
extern void higIoShutdown__Fv(void);
extern void MCPI_InitializeMcpGlobals__Fv(void);
extern int snInitDllSystem(int *header_out);
extern int GetThreadId(void);
extern int ChangeThreadPriority(int thid, int priority);
extern void MCPI_PowerLock__Fv(void);
extern void MCPI_PowerUnlock__Fv(void);
extern void SYSTEM_InitializeMcp__Fi(int size);
extern void SYSTEM_InitGraphicsMCP__Fi(int unused);
extern int TEXMGR_LoadTextureFromMemory__FPCUcUibN22P29tTEXMGR_ALPHA_COLOR_TOLERANCE(
    const unsigned char *data, unsigned int size, int c, int d, int e, int f);
extern void SYSTEM_BeginFrame__Fb(unsigned char mode);
extern void TEXMGR_SetCurrent__FUi(unsigned int tex);
extern void UTIL_DrawTexturedRect__FssssffffUiUi(short x, short y, short w, short h,
                                                 float u0, float v0, float u1, float v1,
                                                 unsigned int c0, unsigned int c1);
extern void SYSTEM_EndFrame__FbUi(unsigned char mode, unsigned int color);
extern int AllocateBufferCache__FUi(unsigned int size);
extern void MEMCARD_Init__FP12tMEMCARD_CMD(tMEMCARD_CMD *cmd);
extern int func_01E65E48(int argc, char **argv);
extern void func_01E65EF0(void);
extern void MCP_Update__Fv(void);
extern void MEMCARD_Update__Fv(void);
extern void MEM_DeinitializeMemory__Fv(void);

extern unsigned int cdloading_gim_size;
extern unsigned char g_MainThreadOn;
extern unsigned char cdloading_gim[];
extern tMEMCARD_CMD D_01F5F700;

int main(int argc, char **argv) {
    unsigned int frame;
    int tex;

    if (APP_Init__Fv() == 0) {
        higIoShutdown__Fv();
        return 0;
    }
    higIoInit__Fv();
    MCPI_InitializeMcpGlobals__Fv();
    if (snInitDllSystem(0) != 0) {
        higIoShutdown__Fv();
        return 0;
    }
    frame = 0;
    ChangeThreadPriority(GetThreadId(), 0x2C);
    MCPI_PowerLock__Fv();
    SYSTEM_InitializeMcp__Fi(0x16000);
    SYSTEM_InitGraphicsMCP__Fi(0);
    tex = TEXMGR_LoadTextureFromMemory__FPCUcUibN22P29tTEXMGR_ALPHA_COLOR_TOLERANCE(
        cdloading_gim, cdloading_gim_size, 1, 0, 0, 0);
    do {
        SYSTEM_BeginFrame__Fb(1);
        frame++;
        TEXMGR_SetCurrent__FUi(tex);
        UTIL_DrawTexturedRect__FssssffffUiUi(400, 336, 64, 64, 1.5f, 1.5f, 62.5f, 62.5f,
                                             0xFFCFBFBF, 0);
        SYSTEM_EndFrame__FbUi(1, 0xFFFFFFFF);
    } while (frame < 2);
    AllocateBufferCache__FUi(0x200000);
    MEMCARD_Init__FP12tMEMCARD_CMD(&D_01F5F700);
    if (func_01E65E48(argc, argv) != 0) {
        MCPI_PowerUnlock__Fv();
        while (g_MainThreadOn != 0) {
            MCP_Update__Fv();
            MEMCARD_Update__Fv();
        }
        func_01E65EF0();
        MEM_DeinitializeMemory__Fv();
    }
    higIoShutdown__Fv();
    return 0;
}
