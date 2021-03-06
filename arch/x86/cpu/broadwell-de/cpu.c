/*
 * Copyright (C) 2017, WarOfDevil <warofdevil@linux.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <cpu.h>
#include <asm/cpu_x86.h>
#include <asm/msr.h>

#define MSR_CORE_THREAD_COUNT 0x35

static void configure_mca(void)
{
	msr_t msr;
	const unsigned int mcg_cap_msr = 0x179;
	int i;
	int num_banks;

	msr = msr_read(mcg_cap_msr);
	num_banks = msr.lo & 0xff;
	msr.lo = 0;
	msr.hi = 0;
	/*
	 * TODO(adurbin): This should only be done on a cold boot. Also, some
	 * of these banks are core vs package scope. For now every CPU clears
	 * every bank
	 */
	for (i = 0; i < num_banks; i++) {
		msr_write(MSR_IA32_MC0_STATUS + (i * 4) + 1, msr);
		msr_write(MSR_IA32_MC0_STATUS + (i * 4) + 2, msr);
		msr_write(MSR_IA32_MC0_STATUS + (i * 4) + 3, msr);

	}
	
	msr.lo = 0xffffffff;
	msr.hi = 0xffffffff;

	for (i = 0; i < num_banks; i++) {
		msr_write(MSR_IA32_MC0_STATUS + (i * 4), msr);
	}
}

static int cpu_x86_broadwell_de_probe(struct udevice *dev)
{	
	debug("Init Broadwell-DE core\n");

	/* Clear out pending MCEs */
	configure_mca();

	return 0;
}

static int broadwell_de_get_info(struct udevice *dev, struct cpu_info *info)
{
	//TBD
  
    return 0;
}

static int broadwell_de_get_count(struct udevice *dev)
{
	msr_t core_thread_count = msr_read(MSR_CORE_THREAD_COUNT);
	return core_thread_count.lo & 0xffff;
}

static const struct cpu_ops cpu_x86_broadwell_de_ops = {
	.get_desc	= cpu_x86_get_desc,
	.get_info	= broadwell_de_get_info,
	.get_count	= broadwell_de_get_count,
	.get_vendor	= cpu_x86_get_vendor,
};

static const struct udevice_id cpu_x86_broadwell_de_ids[] = {
	{ .compatible = "intel,broadwell-de-cpu" },
	{ }
};

U_BOOT_DRIVER(cpu_x86_broadwell_de_drv) = {
	.name		= "cpu_x86_broadwell_de",
	.id		= UCLASS_CPU,
	.of_match	= cpu_x86_broadwell_de_ids,
	.bind		= cpu_x86_bind,
	.probe		= cpu_x86_broadwell_de_probe,
	.ops		= &cpu_x86_broadwell_de_ops,
};
