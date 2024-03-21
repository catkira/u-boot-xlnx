// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#include <common.h>
#include <command.h>
#include <display_options.h>
#include <timestamp.h>
#include <version.h>
#include <version_string.h>
#include <linux/compiler.h>
#include <env.h>
#ifdef CONFIG_SYS_COREBOOT
#include <asm/cb_sysinfo.h>
#endif

#define U_BOOT_VERSION_STRING U_BOOT_VERSION " (" U_BOOT_DATE " - " \
	U_BOOT_TIME " " U_BOOT_TZ ")" CONFIG_IDENT_STRING

const char version_string[] = U_BOOT_VERSION_STRING;

static int do_version(struct cmd_tbl *cmdtp, int flag, int argc,
		      char *const argv[])
{
	char buf[DISPLAY_OPTIONS_BANNER_LENGTH];

	printf(display_options_get_banner(false, buf, sizeof(buf)));
#ifdef CC_VERSION_STRING
	puts(CC_VERSION_STRING "\n");
#endif
#ifdef LD_VERSION_STRING
	puts(LD_VERSION_STRING "\n");
#endif
#ifdef CONFIG_SYS_COREBOOT
	printf("coreboot-%s (%s)\n", lib_sysinfo.version, lib_sysinfo.build);
#endif
	return 0;
}

U_BOOT_CMD(
	version,	1,		1,	do_version,
	"print monitor, compiler and linker version",
	""
);

static int do_env_version(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	env_set("uboot-version", version_string);
	return 0;
}

U_BOOT_CMD(
	envversion,	1,		1,	do_env_version,
	"set env variable uboot-version",
	""
);

#include "xadcps.h"
static int do_xadc(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	static XAdcPs XAdcInst;      /* XADC driver instance */
	XAdcPs_Config *ConfigPtr;
	XAdcPs *XAdcInstPtr = &XAdcInst;
	u32 RawData, chan, cnt = 2;
	int val;
	char buf[16];


	ConfigPtr = XAdcPs_LookupConfig(0);
	if (ConfigPtr == NULL) {
		return -1;
	}

	if (argc > 1)
		chan = simple_strtoul(argv[1], NULL, 10);
	else
		chan = XADCPS_CH_VPVN;

	do {
		XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

		XAdcPs_SetSingleChParams(XAdcInstPtr,chan , 0, 0, 0);
		XAdcPs_SetSequencerMode(XAdcInstPtr, XADCPS_SEQ_MODE_SINGCHAN);

		RawData = XAdcPs_GetAdcData(XAdcInstPtr, chan);

	} while (cnt--);

	XAdcPs_SetPowerdownMode(XAdcInstPtr, XADCPS_PD_MODE_XADC);

	val = ((RawData >> 4) * 1000) / 0xFFF;

	snprintf(buf, sizeof(buf), "%d", val);

	printf("%s\n", buf);
	return val;
}

U_BOOT_CMD(
	xadc, CONFIG_SYS_MAXARGS, 1, do_xadc,
	"read xadc channel",
	""
);

extern int pluto_revA;

static int do_adi_hw_version(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	int val, ret = 0, i;
	char buf[16];

	if (pluto_revA) {
		env_set("PlutoRevA", "1");
	} else {
		val = do_xadc(cmdtp, flag, 1, argv);
		env_set("PlutoRevA", "");
		for (i = 100; i <= 1000; i += 100) {
			if ((val >= (i - 50)) && (val < (i + 50))) {
				ret = i / 100;
				break;
			}
		}
	}

	snprintf(buf, sizeof(buf), "config@%d", ret);
	env_set("fit_config", buf);

	return ret;
}

U_BOOT_CMD(
	adi_hwref, CONFIG_SYS_MAXARGS, 1, do_adi_hw_version,
	"determine pluto/m2k hw revision",
	""
);
