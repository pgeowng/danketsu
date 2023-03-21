#define AlignUpPow2(x, p) (((x) + (p)-1) & ~((p)-1))
#define AlignDownPow2(x, p) ((x) & ~((p)-1))
#define ArrayCount(a) (sizeof(a) / sizeof(*(a)))

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define ClampTop(a, b) Min(a, b)

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#include <stdint.h>

typedef unsigned int uint;
typedef uint64_t U64;
typedef uint8_t U8;

#include <string.h>
#define MemoryZero(p, z) memset((p), 0, (z))