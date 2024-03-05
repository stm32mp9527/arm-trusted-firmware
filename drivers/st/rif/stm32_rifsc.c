/*
 * Copyright (c) 2023-2024, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <drivers/st/stm32_rifsc.h>
#include <drivers/st/stm32mp_rifsc_regs.h>
#include <dt-bindings/soc/rif.h>
#include <lib/mmio.h>

#include <platform_def.h>

static unsigned long rifsc_semaphores[] = {
#if STM32MP21
	STM32MP21_RIFSC_RNG2_ID,
	STM32MP21_RIFSC_PKA_ID,
	STM32MP21_RIFSC_SAES_ID,
#endif /* STM32MP21 */
#if STM32MP23
	STM32MP23_RIFSC_RNG_ID,
	STM32MP23_RIFSC_PKA_ID,
	STM32MP23_RIFSC_SAES_ID,
#endif /* STM32MP23 */
#if STM32MP25
	STM32MP25_RIFSC_RNG_ID,
	STM32MP25_RIFSC_PKA_ID,
	STM32MP25_RIFSC_SAES_ID,
#endif /* STM32MP25 */
};

int stm32_rifsc_semaphore_init(void)
{
	unsigned long i = 0;

	for (i = 0; i < ARRAY_SIZE(rifsc_semaphores); i++) {
		uint32_t cidcfgr = mmio_read_32(RIFSC_BASE +
						_RIFSC_RISC_PERy_CIDCFGR(rifsc_semaphores[i]));
		uint32_t semcfgr = mmio_read_32(RIFSC_BASE +
						_RIFSC_RISC_PERy_SEMCR(rifsc_semaphores[i]));

		uint32_t sem_wl = (cidcfgr & _RIFSC_CIDCFGR_SEML_MASK) >> _RIFSC_CIDCFGR_SEML_SHIFT;

		if (!(((cidcfgr & _RIFSC_CIDCFGR_CFEN) != 0U) &&
		      ((cidcfgr & _RIFSC_CIDCFGR_SEM_EN) != 0U) &&
		      ((sem_wl & RIF_CID1_BF) != RIF_CID1_BF))) {
			continue;
		}

		if (((semcfgr & _RIFSC_SEMCR_SEM_MUTEX) != 0U) &&
		    ((semcfgr & _RIFSC_SEMCR_SEMCID_MASK) >> _RIFSC_SEMCR_SEMCID_SHIFT) != RIF_CID1) {
			return -EACCES;
		}

		mmio_write_32(RIFSC_BASE + _RIFSC_RISC_PERy_SEMCR(rifsc_semaphores[i]),
			      _RIFSC_SEMCR_SEM_MUTEX);

		if (((semcfgr & _RIFSC_SEMCR_SEM_MUTEX) != 0U) &&
		    ((semcfgr & _RIFSC_SEMCR_SEMCID_MASK) >> _RIFSC_SEMCR_SEMCID_SHIFT) != RIF_CID1) {
			return -EACCES;
		}
	}

	return 0;
}

void stm32_rifsc_ip_configure(int rimu_id, int rifsc_id, uint32_t param)
{
	uint32_t bit;

#if STM32MP21
	assert(rifsc_id < STM32MP21_RIFSC_MAX_ID);
#endif /* STM32MP21 */
#if STM32MP23
	assert(rifsc_id < STM32MP23_RIFSC_MAX_ID);
#endif /* STM32MP23 */
#if STM32MP25
	assert(rifsc_id < STM32MP25_RIFSC_MAX_ID);
#endif /* STM32MP25 */

	bit = BIT(rifsc_id / U(32));

	/* Set peripheral accesses to Secure/Privilege only */
	mmio_write_32(RIFSC_BASE + _RIFSC_RISC_SECCFGR(rifsc_id), bit);
	mmio_write_32(RIFSC_BASE + _RIFSC_RISC_PRIVCFGR(rifsc_id), bit);

	/* Apply specific configuration to RIF master */
	mmio_write_32(RIFSC_BASE + _RIFSC_RIMC_ATTR(rimu_id), param);
}
