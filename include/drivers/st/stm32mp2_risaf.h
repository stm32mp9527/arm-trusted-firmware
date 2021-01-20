/*
 * Copyright (c) 2021-2024, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32MP2_RISAF_H
#define STM32MP2_RISAF_H

/* RISAF encryption key size in bytes */
#define RISAF_ENCRYPTION_KEY_SIZE_IN_BYTES	U(16)

int stm32mp2_risaf_write_encryption_key(int instance, uint8_t *key);
int stm32mp2_risaf_lock(int instance);
int stm32mp2_risaf_is_locked(int instance, bool *state);
int stm32mp2_risaf_init(void);

#endif /* STM32MP2_RISAF_H */
