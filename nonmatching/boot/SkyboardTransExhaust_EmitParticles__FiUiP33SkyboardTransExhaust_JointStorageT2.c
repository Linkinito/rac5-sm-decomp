/* USA SCUS_976.15 boot: SkyboardTransExhaust_EmitParticles__FiUiP33SkyboardTransExhaust_JointStorageT2, 0x01E721A0.
 * Observed body: 688 bytes, filling its interval (ends with the return and its
 * delay slot). Measured 28 November 2026: all 688 body bytes are exact after an
 * independent link at the retail address under rebuilt gnu-ee-trial, -O2 -G0,
 * default GNU assembler; the default pinned route gives 365 of 680 bytes. Local exactness
 * with rebuilt tools is not qualification, promotion or C matching credit.
 * Reconstructed from the retail boot. For each of `count` joints (at most two)
 * it takes the step from the current joint position to the target position,
 * records its length and rescales the step to 0.125 unless it is shorter than
 * 2^-22; it emits for every joint at the target positions, then, while either remaining
 * length exceeds 0.25, advances the current joints by one step, shortens both
 * lengths by 0.125 and emits for two joints at the current positions.
 * Source shape that mattered: the vector work goes through small inline
 * helpers taking pointers (subtract, length, set-length with the 0.125 target
 * as a parameter, add), which keeps the per-joint pointers of the retail and
 * leaves the constants inside the loop; the lengths are zeroed by an
 * initializer (a memset call), and each loop has its own counter, the
 * emission counter being signed.
 */
typedef struct {
    float x, y, z;
} VEC3;

typedef struct {
    unsigned char pad_00[0x38];
    VEC3 pos;
    unsigned char pad_44[4];
} SkyboardTransExhaust_JointStorage;

extern float sqrtf(float);
extern void SkyboardTransExhaust_EmitParticles__FiUiUiP33SkyboardTransExhaust_JointStorage(int a, unsigned int b, unsigned int count, SkyboardTransExhaust_JointStorage *storage);

static __inline__ void VEC3_Sub(VEC3 *r, const VEC3 *a, const VEC3 *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

static __inline__ void VEC3_Add(VEC3 *r, const VEC3 *a, const VEC3 *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
    r->z = a->z + b->z;
}

static __inline__ float VEC3_Length(const VEC3 *v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

static __inline__ void VEC3_SetLength(VEC3 *v, float len)
{
    float len2 = v->x * v->x + v->y * v->y + v->z * v->z;
    float s;

    if (len2 < 2.3841858e-07f) {
        return;
    }
    s = len / sqrtf(len2);
    v->x = v->x * s;
    v->y = v->y * s;
    v->z = v->z * s;
}

void SkyboardTransExhaust_EmitParticles__FiUiP33SkyboardTransExhaust_JointStorageT2(int a, unsigned int count, SkyboardTransExhaust_JointStorage *target, SkyboardTransExhaust_JointStorage *current)
{
    VEC3 step[2];
    float dist[2] = { 0.0f, 0.0f };
    unsigned int i;
    unsigned int k;
    int j;

    for (i = 0; i < count; i++) {
        VEC3_Sub(&step[i], &target[i].pos, &current[i].pos);
        dist[i] = VEC3_Length(&step[i]);
        VEC3_SetLength(&step[i], 0.125f);
    }
    for (j = 0; j < count; j++) {
        SkyboardTransExhaust_EmitParticles__FiUiUiP33SkyboardTransExhaust_JointStorage(a, j, count, target);
    }
    while (dist[0] > 0.25f || dist[1] > 0.25f) {
        for (k = 0; k < count; k++) {
            VEC3_Add(&current[k].pos, &current[k].pos, &step[k]);
            dist[k] = dist[k] - 0.125f;
        }
        for (j = 0; j < 2; j++) {
            SkyboardTransExhaust_EmitParticles__FiUiUiP33SkyboardTransExhaust_JointStorage(a, j, count, current);
        }
    }
}
