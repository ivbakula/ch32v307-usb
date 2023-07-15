#ifndef UTILS_MMIO_H
#define UTILS_MMIO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

inline void mmio_write_u32(uintptr_t address, uint32_t data) __attribute__((always_inline));
inline void mmio_write_u16(uintptr_t address, uint16_t data) __attribute__((always_inline));
inline void mmio_write_u8(uintptr_t address, uint16_t data) __attribute__((always_inline));

inline uint32_t mmio_read_u32(uintptr_t address) __attribute__((always_inline));
inline uint16_t mmio_read_u16(uintptr_t address) __attribute__((always_inline));
inline uint8_t  mmio_read_u8(uintptr_t address)  __attribute__((always_inline));

inline void mmio_write_wmask_u32(uintptr_t address, uint32_t mask, uint32_t data) __attribute__((always_inline));
inline void mmio_write_wmask_u16(uintptr_t address, uint16_t mask, uint16_t data) __attribute__((always_inline));
inline void mmio_write_wmask_u8(uintptr_t address, uint8_t mask, uint8_t data) __attribute__((always_inline));

#endif
