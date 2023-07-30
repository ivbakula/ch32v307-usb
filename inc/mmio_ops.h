#ifndef INC_MMIO_H
#define INC_MMIO_H

#include <stdint.h>
#include <stddef.h>

/**
 * @fn mmio_or_writeb
 *
 * @brief Write to one byte wide MMIO register using bitwise or.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_or_writeb(uintptr_t base, uint8_t data);

/**
 * @fn mmio_or_writew
 *
 * @brief Write to one word (16 bits) wide MMIO register using bitwise or.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_or_writew(uintptr_t base, uint16_t data);

/**
 * @fn mmio_or_writel
 *
 * @brief Write to one double word (32 bits) wide MMIO register using bitwise or.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_or_writedw(uintptr_t base, uint32_t data);

/**
 * @fn mmio_and_writeb
 *
 * @brief Write to one byte wide MMIO register using bitwise and.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_and_writeb(uintptr_t base, uint8_t data);

/**
 * @fn mmio_and_writew
 *
 * @brief Write to one word wide MMIO register using bitwise and.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_and_writew(uintptr_t base, uint16_t data);

/**
 * @fn mmio_and_writedw
 *
 * @brief Write to one double word wide MMIO register using bitwise and.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write. 
 */
void mmio_and_writedw(uintptr_t base, uint32_t data);

/**
 * @fn mmio_writeb
 *
 * @brief Write to one byte wide MMIO register using plain ol' assignment operator.
 *        Note this function will overwrite whole register, so if you need to write
 *        only specific bits of a register, please use either mmio_and_write or
 *        mmio_or_write variation of this function.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_writeb(uintptr_t base, uint8_t data);

/**
 * @fn mmio_writew
 *
 * @brief Write to one word wide MMIO register using plain ol' assignment operator.
 *        Note this function will overwrite whole register, so if you need to write
 *        only specific bits of a register, please use either mmio_and_write or
 *        mmio_or_write variation of this function.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_writew(uintptr_t base, uint16_t data);

/**
 * @fn mmio_writedw
 *
 * @brief Write to one double word wide MMIO register using plain ol' assignment operator.
 *        Note this function will overwrite whole register, so if you need to write
 *        only specific bits of a register, please use either mmio_and_write or
 *        mmio_or_write variation of this function.
 *
 * @param base: Base address of MMIO register.
 * @param data: Data to write.
 */
void mmio_writedw(uintptr_t base, uint32_t data);

/**
 * @fn mmio_readb
 *
 * @brief Read one byte wide MMIO register.
 *
 * @param base: base address of MMIO register.
 */
uint8_t mmio_readb(uintptr_t base);

/**
 * @fn mmio_readw
 *
 * @brief Read one word (16 bits) wide MMIO register.
 * 
 * @param base: base address of MMIO register.
 */
uint16_t mmio_readw(uintptr_t base);

/**
 * @fn mmio_readdw
 *
 * @brief Read one double word (32 bits) wide MMIO register.
 *
 * @param base: base address of MMIO register.
 */
uint32_t mmio_readdw(uintptr_t base);

#endif
