#ifndef UTILS_HELPERS_H
#define UTILS_HELPERS_H

#define U8_BIT(x)  ((uint8_t)(1 << x))
#define U16_BIT(x) ((uint16_t)(1 << x))
#define U32_BIT(x) ((uint32_t)(1 << x))

#define UINT8_T(x)   ((uint8_t *)x)
#define UINT16_T(x)  ((uint16_t *)x)
#define UINT32_T(x)  ((uint32_t *)x)
#define UINTPTR_T(x) ((uintptr_t)&x)

#define DIV_4(x) (x >> 2)
#define MUL_4(x) (x << 2)

#endif
