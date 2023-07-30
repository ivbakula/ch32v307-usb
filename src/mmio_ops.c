#include "mmio_ops.h"

#define UINT8_T(x) ((uint8_t *)x)
#define UINT16_T(x) ((uint16_t *)x)
#define UINT32_T(x) ((uint32_t *)x)

inline void mmio_or_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_or_writeb(uintptr_t base, uint8_t data)
{
  *UINT8_T(base) |= data;
}

inline void mmio_or_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_or_writew(uintptr_t base, uint16_t data)
{
  *UINT16_T(base) |= data;
}

inline void mmio_or_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_or_writedw(uintptr_t base, uint32_t data)
{
  *UINT32_T(base) |= data;
}

inline void mmio_and_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_and_writeb(uintptr_t base, uint8_t data)
{
  *UINT8_T(base) &= data;
}

inline void mmio_and_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_and_writew(uintptr_t base, uint16_t data)
{
  *UINT16_T(base) &= data;
}

inline void mmio_and_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_and_writedw(uintptr_t base, uint32_t data)
{
  *UINT32_T(base) &= data;
}

inline void mmio_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_writeb(uintptr_t base, uint8_t data)
{
  *UINT8_T(base) = data;
}

inline void mmio_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_writew(uintptr_t base, uint16_t data)
{
  *UINT16_T(base) = data;
}

inline void mmio_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_writedw(uintptr_t base, uint32_t data)
{
  *UINT32_T(base) = data;
}

inline uint8_t mmio_readb(uintptr_t base) __attribute__((always_inline));
inline uint8_t mmio_readb(uintptr_t base)
{
  return *UINT8_T(base);
}

inline uint16_t mmio_readw(uintptr_t base) __attribute__((always_inline));
inline uint16_t mmio_readw(uintptr_t base)
{
  return *UINT16_T(base);
}

inline uint32_t mmio_readdw(uintptr_t base) __attribute__((always_inline));
inline uint32_t mmio_readdw(uintptr_t base)
{
  return *UINT32_T(base);
}
