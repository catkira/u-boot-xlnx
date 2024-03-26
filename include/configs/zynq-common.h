/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2012 Michal Simek <monstr@monstr.eu>
 * (C) Copyright 2013 - 2018 Xilinx, Inc.
 *
 * Common configuration options for all Zynq boards.
 */

#ifndef __CONFIG_ZYNQ_COMMON_H
#define __CONFIG_ZYNQ_COMMON_H

/* CPU clock */
#ifndef CONFIG_CPU_FREQ_HZ
# define CONFIG_CPU_FREQ_HZ	800000000
#endif

#define CONFIG_REMAKE_ELF

/* Cache options */
#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
# define CONFIG_SYS_L2_PL310
# define CONFIG_SYS_PL310_BASE		0xf8f02000
#endif

#define ZYNQ_SCUTIMER_BASEADDR		0xF8F00600
#define CONFIG_SYS_TIMERBASE		ZYNQ_SCUTIMER_BASEADDR
#define CONFIG_SYS_TIMER_COUNTS_DOWN
#define CONFIG_SYS_TIMER_COUNTER	(CONFIG_SYS_TIMERBASE + 0x4)

/* Serial drivers */
/* The following table includes the supported baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE  \
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400}

#define CONFIG_ARM_DCC

/* Ethernet driver */
#if defined(CONFIG_ZYNQ_GEM)
# define CONFIG_SYS_FAULT_ECHO_LINK_DOWN
# define CONFIG_BOOTP_MAY_FAIL
#endif

/* QSPI */
#ifdef CONFIG_ZYNQ_QSPI
# define CONFIG_SF_DEFAULT_SPEED	50000000
#endif

# define DFU_ALT_INFO_SF \
"dfu_sf_info="\
"set dfu_alt_info " \
"${boot_image} raw 0x0 0xE0000\\\\;" \
"${kernel_image} raw 0x100000 0x500000\\\\;" \
"${devicetree_image} raw 0x600000 0x20000\\\\;" \
"${ramdisk_image} raw 0x620000 0xCE0000\\\\;" \
"${bitstream_image} raw 0x1300000 0xD00000\0" \
"dfu_sf=run dfu_sf_info && dfu 0 sf 0:0:40000000:0\0"

# define DFU_ALT_INFO_SF1 \
"dfu_sf_info="\
"set dfu_alt_info " \
"boot.dfu raw 0x0 0x100000\\\\;" \
"firmware.dfu raw 0x200000 0x1E00000\\\\;" \
"uboot-extra-env.dfu raw 0xFF000 0x1000\\\\;" \
"uboot-env.dfu raw 0x100000 0x20000\\\\;" \
"spare.dfu raw 0x120000 0xE0000\0" \
"dfu_sf=gpio set 15;set stdout serial@e0001000;echo Entering DFU SF mode ... && run dfu_sf_info && dfu 0 sf 0:0:40000000:0 && if test -n ${dfu_alt_num} && test ${dfu_alt_num} = 1; "\
"then set fit_size ${filesize} && set dfu_alt_num && env save; fi;gpio clear 15;\0"


/* NOR */
#ifdef CONFIG_MTD_NOR_FLASH
# define CONFIG_SYS_FLASH_BASE		0xE2000000
# define CONFIG_SYS_FLASH_SIZE		(16 * 1024 * 1024)
# define CONFIG_SYS_MAX_FLASH_BANKS	1
# define CONFIG_SYS_MAX_FLASH_SECT	512
# define CONFIG_SYS_FLASH_ERASE_TOUT	1000
# define CONFIG_SYS_FLASH_WRITE_TOUT	5000
# define CONFIG_FLASH_SHOW_PROGRESS	10
# undef CONFIG_SYS_FLASH_EMPTY_INFO
#endif

#ifdef CONFIG_NAND_ZYNQ
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#endif

#ifdef CONFIG_USB_EHCI_ZYNQ
# define CONFIG_EHCI_IS_TDI

# define CONFIG_SYS_DFU_DATA_BUF_SIZE	0x600000
# define DFU_DEFAULT_POLL_TIMEOUT	300
# define CONFIG_THOR_RESET_OFF
# define DFU_ALT_INFO_RAM \
	"dfu_ram_info=" \
	"set dfu_alt_info " \
	"dummy.dfu ram 0 0\\\\;" \
	"firmware.dfu ram ${fit_load_address} 0x1E00000\0" \
	"dfu_ram=echo Entering DFU RAM mode ... && run dfu_ram_info && dfu 0 ram 0\0" \
	"thor_ram=run dfu_ram_info && thordown 0 ram 0\0"

# if defined(CONFIG_MMC_SDHCI_ZYNQ)
#  define DFU_ALT_INFO_MMC \
	"dfu_mmc_info=" \
	"set dfu_alt_info " \
	"${kernel_image} fat 0 1\\\\;" \
	"${devicetree_image} fat 0 1\\\\;" \
	"${ramdisk_image} fat 0 1\0" \
	"dfu_mmc=run dfu_mmc_info && dfu 0 mmc 0\0" \
	"thor_mmc=run dfu_mmc_info && thordown 0 mmc 0\0"

#  define DFU_ALT_INFO	\
	DFU_ALT_INFO_RAM \
	DFU_ALT_INFO_MMC
# else
#  define DFU_ALT_INFO	\
	DFU_ALT_INFO_RAM
# endif
#endif

#if !defined(DFU_ALT_INFO)
# define DFU_ALT_INFO
#endif

/* I2C */
#if defined(CONFIG_SYS_I2C_ZYNQ)
# define CONFIG_SYS_I2C
#endif

/* EEPROM */
#ifdef CONFIG_ENV_IS_IN_EEPROM
# define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
# define CONFIG_SYS_I2C_EEPROM_ADDR		0x54
# define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	4
# define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5
# define CONFIG_SYS_EEPROM_SIZE			1024 /* Bytes */
# define CONFIG_SYS_I2C_MUX_ADDR		0x74
# define CONFIG_SYS_I2C_MUX_EEPROM_SEL		0x4

/* Total Size of Environment Sector */
# define CONFIG_EXTRA_ENV_SETTINGS
#endif

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* enable preboot to be loaded before CONFIG_BOOTDELAY */
#define CONFIG_PREBOOT

/* Boot configuration */
#define CONFIG_SYS_LOAD_ADDR		0 /* default? */

#ifdef CONFIG_SPL_BUILD
#define BOOTENV
#else

#ifdef CONFIG_CMD_MMC
#define BOOT_TARGET_DEVICES_MMC(func) func(MMC, mmc, 0)
#else
#define BOOT_TARGET_DEVICES_MMC(func)
#endif

#ifdef CONFIG_CMD_USB
#define BOOT_TARGET_DEVICES_USB(func) func(USB, usb, 0)
#else
#define BOOT_TARGET_DEVICES_USB(func)
#endif

#if defined(CONFIG_CMD_PXE) && defined(CONFIG_CMD_DHCP)
#define BOOT_TARGET_DEVICES_PXE(func) func(PXE, pxe, na)
#else
#define BOOT_TARGET_DEVICES_PXE(func)
#endif

#if defined(CONFIG_CMD_DHCP)
#define BOOT_TARGET_DEVICES_DHCP(func) func(DHCP, dhcp, na)
#else
#define BOOT_TARGET_DEVICES_DHCP(func)
#endif

#if defined(CONFIG_ZYNQ_QSPI)
# define BOOT_TARGET_DEVICES_QSPI(func)	func(QSPI, qspi, na)
#else
# define BOOT_TARGET_DEVICES_QSPI(func)
#endif

#if defined(CONFIG_NAND_ZYNQ)
# define BOOT_TARGET_DEVICES_NAND(func)	func(NAND, nand, na)
#else
# define BOOT_TARGET_DEVICES_NAND(func)
#endif

#if defined(CONFIG_MTD_NOR_FLASH)
# define BOOT_TARGET_DEVICES_NOR(func)	func(NOR, nor, na)
#else
# define BOOT_TARGET_DEVICES_NOR(func)
#endif

#define BOOTENV_DEV_XILINX(devtypeu, devtypel, instance) \
	"bootcmd_xilinx=run $modeboot\0"

#define BOOTENV_DEV_NAME_XILINX(devtypeu, devtypel, instance) \
	"xilinx "

#define BOOTENV_DEV_QSPI(devtypeu, devtypel, instance) \
	"bootcmd_qspi=sf probe 0 0 0 && " \
		      "sf read $scriptaddr $script_offset_f $script_size_f && " \
		      "source ${scriptaddr}; echo SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_QSPI(devtypeu, devtypel, instance) \
	"qspi "

#define BOOTENV_DEV_NAND(devtypeu, devtypel, instance) \
	"bootcmd_nand=nand info && " \
		      "nand read $scriptaddr $script_offset_f $script_size_f && " \
		      "source ${scriptaddr}; echo SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_NAND(devtypeu, devtypel, instance) \
	"nand "

#define BOOTENV_DEV_NOR(devtypeu, devtypel, instance) \
	"bootcmd_nor=cp.b $scropt_offset_nor $scriptaddr $script_size_f && " \
		     "source ${scriptaddr}; echo SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_NOR(devtypeu, devtypel, instance) \
	"nor "

#define BOOT_TARGET_DEVICES(func) \
	BOOT_TARGET_DEVICES_MMC(func) \
	BOOT_TARGET_DEVICES_QSPI(func) \
	BOOT_TARGET_DEVICES_NAND(func) \
	BOOT_TARGET_DEVICES_NOR(func) \
	BOOT_TARGET_DEVICES_USB(func) \
	BOOT_TARGET_DEVICES_PXE(func) \
	BOOT_TARGET_DEVICES_DHCP(func) \
	func(XILINX, xilinx, na)

#include <config_distro_bootcmd.h>
#endif /* CONFIG_SPL_BUILD */

/* Default environment */
#ifndef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"ethaddr=00:0a:35:00:01:22\0"	\
	"ipaddr=192.168.2.1\0"	\
	"ipaddr_host=192.168.2.10\0"	\
	"ipaddr_eth=192.168.137.2\0"	\
	"netmask=255.255.255.0\0"	\
	"kernel_image=uImage\0"	\
	"fit_load_address=0x5000000\0" \
	"fit_config=config@0\0" \
	"extraenv_load_address=0x207E000\0" \
	"ramdisk_image=uramdisk.image.gz\0"	\
	"ramdisk_load_address=0x4000000\0"	\
	"devicetree_image=devicetree.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"bitstream_image=system.bit.bin\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbit_addr=0x100000\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"fit_size=0x900000\0"	\
	"devicetree_size=0x20000\0"	\
	"ramdisk_size=0x400000\0"	\
	"bitstream_size=0x400000\0" \
	"boot_size=0xF00000\0"	\
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"scriptaddr=0x20000\0"	\
	"script_offser_nor=0xE2FC0000\0"	\
	"script_offset_f=0xFC0000\0"	\
	"script_size_f=0x40000\0"	\
	"bootenv=uEnv.txt\0" \
	"maxcpus=2\0" \
	"clear_reset_cause=mw f8000008 df0d && mw f8000258 00400000 && mw f8000004 767b\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"sd_uEnvtxt_existence_test=test -e mmc 0 /uEnv.txt\0" \
	"preboot=if test $modeboot = sdboot && env run sd_uEnvtxt_existence_test; " \
			"then if env run loadbootenv; " \
				"then env run importbootenv; " \
			"fi; " \
		"fi; \0" \
	"refclk_source=internal\0" \
	"mode=2r2t\0" \
	"adi_loadvals_pluto=if test -n \"${ad936x_ext_refclk}\" && test ! -n \"${ad936x_skip_ext_refclk}\"; then " \
			"fdt set /clocks/clock@0 clock-frequency ${ad936x_ext_refclk}; " \
		"fi; " \
		"if test -n \"${ad936x_ext_refclk_override}\"; then " \
			"fdt set /clocks/clock@0 clock-frequency ${ad936x_ext_refclk_override}; " \
		"fi; " \
		"if test -n \"${refclk_source}\" && test ! \"${refclk_source}\" = \"internal\" && test ! \"${refclk_source}\" = \"external\"; then " \
			"setenv refclk_source internal; " \
			"saveenv; " \
		"fi; " \
		"if test \"${refclk_source}\" = \"internal\" && test \"${model}\" = \"Analog Devices PlutoSDR Rev.C (Z7010/AD9363)\" ; then " \
			"fdt rm /amba/gpio@e000a000/clock_extern_en || fdt rm /axi/gpio@e000a000/clock_extern_en; " \
		"fi; " \
		"if test \"${refclk_source}\" = \"external\" && test \"${model}\" = \"Analog Devices PlutoSDR Rev.C (Z7010/AD9363)\" ; then " \
			"fdt rm /amba/gpio@e000a000/clock_internal_en || fdt rm /axi/gpio@e000a000/clock_internal_en; " \
		"fi; " \
		"if test  \"${attr_val}\" = \"ad9361\" && test ! \"${model}\" = \"Analog Devices PlutoSDR Rev.C (Z7010/AD9363)\" ; then " \
			"setenv attr_val ad9363a; " \
			"saveenv; " \
		"fi; " \
		"if test -n \"${attr_val}\" && test ! \"${attr_val}\" = \"ad9361\" && test ! \"${attr_val}\" = \"ad9363a\" && test ! \"${attr_val}\" = \"ad9364\"; then " \
			"setenv attr_val ad9363a; " \
			"saveenv; " \
		"fi; " \
		"if test -n \"${mode}\" && test ! \"${mode}\" = \"1r1t\" && test ! \"${mode}\" = \"2r2t\"; then " \
			"setenv mode 1r1t; " \
			"saveenv; " \
		"fi; " \
		"if test -n \"${attr_name}\" && test -n \"${attr_val}\"; then " \
			"fdt set /amba/spi@e0006000/ad9361-phy@0 ${attr_name} ${attr_val} || fdt set /axi/spi@e0006000/ad9361-phy@0 ${attr_name} ${attr_val}; " \
                "fi; " \
		"if test \"${mode}\" = \"1r1t\" && test \"${model}\" = \"Analog Devices PlutoSDR Rev.C (Z7010/AD9363)\"; then " \
			"fdt rm /amba/spi@e0006000/ad9361-phy@0 adi,2rx-2tx-mode-enable || fdt rm /axi/spi@e0006000/ad9361-phy@0 adi,2rx-2tx-mode-enable; " \
			"fdt set /fpga-axi/cf-ad9361-dds-core-lpc@79024000 compatible adi,axi-ad9364-dds-6.00.a; " \
		"fi; " \
		"if test -n \"${cs_gpio}\" && test \"${model}\" = \"Analog Devices PlutoSDR Rev.C (Z7010/AD9363)\"; then " \
			"fdt set /amba/axi_quad_spi@7C430000/ cs-gpios \"<0x06 ${cs_gpio} 0>\" || fdt set /axi/axi_quad_spi@7C430000/ cs-gpios \"<0x06 ${cs_gpio} 0>\"; " \
		"fi; " \
		"if test -n \"${attr_val}\" && test \"${attr_val}\" = \"ad9364\"; then " \
			"fdt set /fpga-axi/cf-ad9361-dds-core-lpc@79024000 compatible adi,axi-ad9364-dds-6.00.a; " \
			"if test ! \"${mode}\" = \"1r1t\"; then " \
				"fdt rm /amba/spi@e0006000/ad9361-phy@0 adi,2rx-2tx-mode-enable || fdt rm /axi/spi@e0006000/ad9361-phy@0 adi,2rx-2tx-mode-enable; " \
				"setenv mode 1r1t; " \
				"saveenv; " \
			"fi; " \
		"fi; \0" \
	"adi_loadvals=fdt addr ${fit_load_address} && fdt get value fdt_choosen /configurations/${fit_config}/ fdt && " \
		"fdt get addr fdtaddr /images/${fdt_choosen} data && fdt addr ${fdtaddr}; "\
		"fdt get value model / model; " \
		"if test \"${model}\" \> \"Analog Devices Pluto\"; then " \
			"run adi_loadvals_pluto; " \
		"fi; \0" \
	"qspiboot_extraenv=sf read ${extraenv_load_address} 0xFF000 0x1000 && " \
		"env import -c ${extraenv_load_address} 0x1000 || true \0" \
	"read_sf=sf probe 0:0 50000000 0 && run qspiboot_extraenv &&" \
		"sf read ${fit_load_address} 0x200000 ${fit_size} && " \
		"iminfo ${fit_load_address} || " \
		"sf read ${fit_load_address} 0x200000  0x1E00000; \0" \
	"ramboot_verbose=adi_hwref;echo Copying Linux from DFU to RAM... && " \
		"run dfu_ram;" \
		"if run adi_loadvals; then " \
		"echo Loaded AD936x refclk frequency and model into devicetree; " \
		"fi; " \
		"envversion;setenv bootargs console=ttyPS0,115200 maxcpus=${maxcpus} rootfstype=ramfs root=/dev/ram0 rw earlyprintk clk_ignore_unused uboot=\"${uboot-version}\" && " \
		"bootm ${fit_load_address}#${fit_config}\0" \
	"qspiboot_verbose=adi_hwref;echo Copying Linux from QSPI flash to RAM... && " \
		"run read_sf && " \
		"if run adi_loadvals; then " \
		"echo Loaded AD936x refclk frequency and model into devicetree; " \
		"fi; " \
		"envversion;setenv bootargs console=ttyPS0,115200 maxcpus=${maxcpus} rootfstype=ramfs root=/dev/ram0 rw earlyprintk clk_ignore_unused uboot=\"${uboot-version}\" && " \
		"bootm ${fit_load_address}#${fit_config} || echo BOOT failed entering DFU mode ... && run dfu_sf \0" \
	"qspiboot=set stdout nulldev;adi_hwref;test -n $PlutoRevA || gpio input 14 && set stdout serial@e0001000 && sf probe && sf protect lock 0 100000 && run dfu_sf;  " \
		"set stdout serial@e0001000;" \
		"itest *f8000258 == 480003 && run clear_reset_cause && run dfu_sf; " \
		"itest *f8000258 == 480007 && run clear_reset_cause && run ramboot_verbose; " \
		"itest *f8000258 == 480006 && run clear_reset_cause && run qspiboot_verbose; " \
		"itest *f8000258 == 480002 && run clear_reset_cause && exit; " \
		"echo Booting silently && set stdout nulldev; " \
		"run read_sf && run adi_loadvals; " \
		"envversion;setenv bootargs console=ttyPS0,115200 maxcpus=${maxcpus} rootfstype=ramfs root=/dev/ram0 rw quiet loglevel=4 clk_ignore_unused uboot=\"${uboot-version}\" && " \
		"bootm ${fit_load_address}#${fit_config} || set stdout serial@e0001000;echo BOOT failed entering DFU mode ... && sf protect lock 0 100000 && run dfu_sf \0" \
	"jtagboot=env default -a;sf probe && sf protect unlock 0 100000 && run dfu_sf; \0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=run xilinxcmd && if mmcinfo; then " \
			"run uenvboot; " \
			"itest *f8000258 == 480003 && run clear_reset_cause && run dfu_sf; " \
			"itest *f8000258 == 480007 && run clear_reset_cause && run ramboot_verbose; " \
			"echo Starting Linux from SD... && " \
			"bootm ${fit_load_address}; " \
		"fi\0" \
	"usbboot=run xilinxcmd && if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"load usb 0 ${fit_load_address} ${kernel_image} && " \
			"load usb 0 ${devicetree_load_address} ${devicetree_image} && " \
			"load usb 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${fit_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	DFU_ALT_INFO \
	DFU_ALT_INFO_SF1
	#endif

/* Miscellaneous configurable options */

#define CONFIG_CLOCKS
#define CONFIG_SYS_MAXARGS		32 /* max number of command args */
#define CONFIG_SYS_CBSIZE		2048 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MEMTEST_START	0
#define CONFIG_SYS_MEMTEST_END		0x1000

#define CONFIG_SYS_INIT_RAM_ADDR	0xFFFF0000
#define CONFIG_SYS_INIT_RAM_SIZE	0x2000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)


/* Extend size of kernel image for uncompression */
#define CONFIG_SYS_BOOTM_LEN	(60 * 1024 * 1024)

/* Boot FreeBSD/vxWorks from an ELF image */
#define CONFIG_SYS_MMC_MAX_DEVICE	1

#define CONFIG_SYS_LDSCRIPT  "arch/arm/mach-zynq/u-boot.lds"

#undef CONFIG_BOOTM_NETBSD

/* MMC support */
#ifdef CONFIG_MMC_SDHCI_ZYNQ
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME     "u-boot.img"
#endif

/* Disable dcache for SPL just for sure */
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_DCACHE_OFF
#endif

/* Address in RAM where the parameters must be copied by SPL. */
#define CONFIG_SYS_SPL_ARGS_ADDR	0x10000000

#define CONFIG_SPL_FS_LOAD_ARGS_NAME		"system.dtb"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME		"uImage"

/* Not using MMC raw mode - just for compilation purpose */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR	0
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS	0
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR	0

/* qspi mode is working fine */
#ifdef CONFIG_ZYNQ_QSPI
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x100000
#define CONFIG_SYS_SPI_ARGS_OFFS	0x200000
#define CONFIG_SYS_SPI_ARGS_SIZE	0x80000
#define CONFIG_SYS_SPI_KERNEL_OFFS	(CONFIG_SYS_SPI_ARGS_OFFS + \
					CONFIG_SYS_SPI_ARGS_SIZE)
#endif

/* SP location before relocation, must use scratch RAM */
#define CONFIG_SPL_TEXT_BASE	0x0

/* 3 * 64kB blocks of OCM - one is on the top because of bootrom */
#define CONFIG_SPL_MAX_SIZE	0x30000

/* On the top of OCM space */
#define CONFIG_SYS_SPL_MALLOC_START	CONFIG_SPL_STACK_R_ADDR
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x2000000

/*
 * SPL stack position - and stack goes down
 * 0xfffffe00 is used for putting wfi loop.
 * Set it up as limit for now.
 */
#define CONFIG_SPL_STACK	0xfffffe00

/* BSS setup */
#define CONFIG_SPL_BSS_START_ADDR	0x100000
#define CONFIG_SPL_BSS_MAX_SIZE		0x100000

#define CONFIG_SPL_LOAD_FIT_ADDRESS 0x10000000

#define CONFIG_SYS_UBOOT_START	CONFIG_SYS_TEXT_BASE

#endif /* __CONFIG_ZYNQ_COMMON_H */
