#include "mmio_ops.h"

#include "helpers.h"

inline void mmio_or_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_or_writeb(uintptr_t base, uint8_t data)
{
  *__IO_UINT8_T(base) |= data;
}

inline void mmio_or_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_or_writew(uintptr_t base, uint16_t data)
{
  *__IO_UINT16_T(base) |= data;
}

inline void mmio_or_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_or_writedw(uintptr_t base, uint32_t data)
{
  *__IO_UINT32_T(base) |= data;
}

inline void mmio_and_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_and_writeb(uintptr_t base, uint8_t data)
{
  *__IO_UINT8_T(base) &= data;
}

inline void mmio_and_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_and_writew(uintptr_t base, uint16_t data)
{
  *__IO_UINT16_T(base) &= data;
}

inline void mmio_and_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_and_writedw(uintptr_t base, uint32_t data)
{
  *__IO_UINT32_T(base) &= data;
}

inline void mmio_writeb(uintptr_t base, uint8_t data) __attribute__((always_inline));
inline void mmio_writeb(uintptr_t base, uint8_t data)
{
  *__IO_UINT8_T(base) = data;
}

inline void mmio_writew(uintptr_t base, uint16_t data) __attribute__((always_inline));
inline void mmio_writew(uintptr_t base, uint16_t data)
{
  *__IO_UINT16_T(base) = data;
}

inline void mmio_writedw(uintptr_t base, uint32_t data) __attribute__((always_inline));
inline void mmio_writedw(uintptr_t base, uint32_t data)
{
  *__IO_UINT32_T(base) = data;
}

inline uint8_t mmio_readb(uintptr_t base) __attribute__((always_inline));
inline uint8_t mmio_readb(uintptr_t base)
{
  return *__IO_UINT8_T(base);
}

inline uint16_t mmio_readw(uintptr_t base) __attribute__((always_inline));
inline uint16_t mmio_readw(uintptr_t base)
{
  return *__IO_UINT16_T(base);
}

inline uint32_t mmio_readdw(uintptr_t base) __attribute__((always_inline));
inline uint32_t mmio_readdw(uintptr_t base)
{
  return *__IO_UINT32_T(base);
}

inline uint8_t mmio_and_readb(uintptr_t base, uint8_t mask) __attribute__((always_inline));
inline uint8_t mmio_and_readb(uintptr_t base, uint8_t mask)
{
  return (*__IO_UINT32_T(base) & mask);
}

inline uint16_t mmio_and_readw(uintptr_t base, uint16_t mask) __attribute__((always_inline));
inline uint16_t mmio_and_readw(uintptr_t base, uint16_t mask)
{
  return (*__IO_UINT32_T(base) & mask);
}

inline uint32_t mmio_and_readdw(uintptr_t base, uint32_t mask) __attribute__((always_inline));
inline uint32_t mmio_and_readdw(uintptr_t base, uint32_t mask)
{
  return (*__IO_UINT32_T(base) & mask);
}
