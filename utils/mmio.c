#include "mmio.h"
#include <stdint.h>

inline void mmio_write_u32(uintptr_t address, uint32_t data)
{
  *((uint32_t *) address) = data;
}

inline void mmio_write_u16(uintptr_t address, uint16_t data)
{
  *((uint16_t *) address) = data;
}

inline void mmio_write_u8(uintptr_t address, uint16_t data)
{
  *((uint8_t *) address) = data;
}

inline uint32_t mmio_read_u32(uintptr_t address)
{
  return *((uint32_t *) address);
}

inline uint16_t mmio_read_u16(uintptr_t address)
{
  return *((uint16_t *) address);
}

inline uint8_t mmio_read_u8(uintptr_t address)
{
  return *((uint16_t *)address);
}

inline void mmio_write_wmask_u32(uintptr_t address, uint32_t mask, uint32_t data)
{
  *((uint32_t *)address) &= ~mask;
  *((uint32_t *)address) |= mask & data; 
}

inline void mmio_write_wmask_u16(uintptr_t address, uint16_t mask, uint16_t data)
{
  *((uint16_t *)address) &= ~mask;
  *((uint16_t *)address) |= mask & data;
}

inline void mmio_write_wmask_u8(uintptr_t address, uint8_t mask, uint8_t data)
{
  *((uint8_t *)address) &= ~mask;
  *((uint8_t *)address) |= mask & data;
}  
