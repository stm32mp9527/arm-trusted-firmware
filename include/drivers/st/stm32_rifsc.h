/*
 * Copyright (c) 2023-2024, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32MP2_RIFSC_H
#define STM32MP2_RIFSC_H

/*
 * stm32_rifsc_semaphore_init() - Takes RIFSC semaphore for some IDs
 *
 * Return 0 on success and -EACCES if authorized semaphore couldn't be taken
 */
int stm32_rifsc_semaphore_init(void);
void stm32_rifsc_ip_configure(int rimu_id, int rifsc_id, uint32_t param);

#endif /* STM32MP2_RIFSC_H */
