/*
 * Copyright (C) 2017, WarOfDevil <warofdevil@linux.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BROADWELL_DE_GLOBAL_NVS_H_
#define _BROADWELL_DE_GLOBAL_NVS_H_

struct __packed acpi_global_nvs {
	u8	pcnt;		/* processor count */
	u8	iuart_en;	/* internal UART enabled */

	/*
	 * Add padding so sizeof(struct acpi_global_nvs) == 0x100.
	 * This must match the size defined in the global_nvs.asl.
	 */
	u8	rsvd[254];
};

#endif /* _BROADWELL_DE_GLOBAL_NVS_H_ */
