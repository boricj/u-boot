/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002, 2010
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/arch/s3c24x0_cpu.h>

DECLARE_GLOBAL_DATA_PTR;

#define FCLK_SPEED 1

#if (FCLK_SPEED == 0)		/* Fout = 203MHz, Fin = 12MHz for Audio */
#define M_MDIV	0xC3
#define M_PDIV	0x4
#define M_SDIV	0x1
#elif (FCLK_SPEED == 1)		/* Fout = 202.8MHz */
#define M_MDIV	0xA1
#define M_PDIV	0x3
#define M_SDIV	0x1
#endif

#define USB_CLOCK 1

#if (USB_CLOCK == 0)
#define U_M_MDIV	0xA1
#define U_M_PDIV	0x3
#define U_M_SDIV	0x1
#elif (USB_CLOCK == 1)
#define U_M_MDIV	0x48
#define U_M_PDIV	0x3
#define U_M_SDIV	0x2
#endif

static inline void pll_delay(unsigned long loops)
{
	__asm__ volatile ("1:\n"
	  "subs %0, %1, #1\n"
	  "bne 1b" : "=r" (loops) : "0" (loops));
}

#define GPCCON    (volatile uint32_t*)0x56000020
#define GPCDAT    (volatile uint32_t*)0x56000024

void coloured_LED_init (void)
{
	*GPCCON |= 0x5400;
	*GPCDAT &= ~0xE0;
}

void blue_led_on(void)
{
	*GPCDAT |= 0x20;
}

void blue_led_off(void)
{
	*GPCDAT &= ~0x20;
}

void green_led_on(void)
{
	*GPCDAT |= 0x40;
}

void green_led_off(void)
{
	*GPCDAT &= ~0x40;
}

void red_led_on (void)
{
	*GPCDAT |= 0x80;
}

void red_led_off(void)
{
	*GPCDAT &= ~0x80;
}

/*
 * Miscellaneous platform dependent initialisations
 */

void lowlevel_init(void) {}

int board_early_init_f(void)
{
	icache_disable();
	dcache_disable();
	cache_flush();

	return 0;
}

int board_init(void)
{
	/* arch number of SMDK2410-Board */
	gd->bd->bi_arch_number = MACH_TYPE_SMDK2410;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x30000100;

	icache_enable();
	dcache_enable();

	return 0;
}

int dram_init(void)
{
	return 0;
}

phys_size_t get_effective_memsize(void)
{
	/* dram_init must store complete ramsize in gd->ram_size */
	return PHYS_SDRAM_1_SIZE;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_CS8900
	rc = cs8900_initialize(0, CONFIG_CS8900_BASE);
#endif
	return rc;
}
#endif
