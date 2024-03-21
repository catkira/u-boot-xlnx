/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2012 Michal Simek <monstr@monstr.eu>
 * (C) Copyright 2013 - 2018 Xilinx, Inc.
 *
 * Common configuration options for all Zynq boards.
 */

#ifndef __CONFIG_ZYNQ_COMMON_H
#define __CONFIG_ZYNQ_COMMON_H

#define CONFIG_MISC_INIT_R
/* CPU clock */
#ifndef CONFIG_CPU_FREQ_HZ
# define CONFIG_CPU_FREQ_HZ	800000000
#endif

#define	CONFIG_SYS_DEVICE_NULLDEV	1

/* Cache options */
#ifndef CONFIG_SYS_L2CACHE_OFF
# define CONFIG_SYS_PL310_BASE		0xf8f02000
#endif

#define ZYNQ_SCUTIMER_BASEADDR		0xF8F00600
#define CONFIG_SYS_TIMERBASE		ZYNQ_SCUTIMER_BASEADDR
#define CONFIG_SYS_TIMER_COUNTS_DOWN
#define CONFIG_SYS_TIMER_COUNTER	(CONFIG_SYS_TIMERBASE + 0x4)

/* GUIDs for capsule updatable firmware images */
#define XILINX_BOOT_IMAGE_GUID \
	EFI_GUID(0x1ba29a15, 0x9969, 0x40aa, 0xb4, 0x24, \
		 0xe8, 0x61, 0x21, 0x61, 0x86, 0x64)

#define XILINX_UBOOT_IMAGE_GUID \
	EFI_GUID(0x1a5178f0, 0x87d3, 0x4f36, 0xac, 0x63, \
		 0x3b, 0x31, 0xa2, 0x3b, 0xe3, 0x05)

/* Serial drivers */
/* The following table includes the supported baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE  \
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400}

/* Ethernet driver */

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
# define CONFIG_FLASH_SHOW_PROGRESS	10
#endif

#ifdef CONFIG_USB_EHCI_ZYNQ
# define DFU_DEFAULT_POLL_TIMEOUT	300
# define CONFIG_THOR_RESET_OFF
# define CONFIG_USB_FUNCTION_THOR
# define DFU_ALT_INFO_RAM \
	"dfu_ram_info=" \
	"set dfu_alt_info " \
	"dummy.dfu ram 0 0\\\\;" \
	"firmware.dfu ram ${fit_load_address} 0x1E00000\0" \
	"dfu_ram=echo Entering DFU RAM mode ... && run dfu_ram_info && dfu 0 ram 0\0" \
	"thor_ram=run dfu_ram_info && thordown 0 ram 0\0"

# if defined(CONFIG_ZYNQ_SDHCI)
#  define CONFIG_DFU_MMC
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

#if defined(CONFIG_ZYNQ_SDHCI) || defined(CONFIG_ZYNQ_USB)
# define CONFIG_SUPPORT_VFAT
# define CONFIG_FAT_WRITE
# define CONFIG_DOS_PARTITION
#endif

/* NAND */
#ifdef CONFIG_NAND_ZYNQ
# define CONFIG_CMD_NAND
# define CONFIG_CMD_NAND_LOCK_UNLOCK
# define CONFIG_SYS_MAX_NAND_DEVICE 1
# define CONFIG_SYS_NAND_SELF_INIT
# define CONFIG_SYS_NAND_ONFI_DETECTION
# define CONFIG_MTD_DEVICE
#endif

#if defined(CONFIG_ZYNQ_I2C0) || defined(CONFIG_ZYNQ_I2C1)
#define CONFIG_SYS_I2C_ZYNQ
#endif

/* I2C */
#if defined(CONFIG_SYS_I2C_ZYNQ)
# define CONFIG_SYS_I2C
# define CONFIG_SYS_I2C_ZYNQ_SPEED		100000
# define CONFIG_SYS_I2C_ZYNQ_SLAVE		0
#endif

/* EEPROM */
#ifdef CONFIG_ZYNQ_EEPROM
# define CONFIG_CMD_EEPROM
# define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
# define CONFIG_SYS_I2C_EEPROM_ADDR		0x54
# define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	4
# define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5
# define CONFIG_SYS_EEPROM_SIZE			1024 /* Bytes */
# define CONFIG_SYS_I2C_MUX_ADDR		0x74
# define CONFIG_SYS_I2C_MUX_EEPROM_SEL		0x4
#endif

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Environment */
#ifndef CONFIG_ENV_IS_NOWHERE
# ifndef CONFIG_SYS_NO_FLASH
/* Environment in NOR flash */
#  define CONFIG_ENV_IS_IN_FLASH
# elif defined(CONFIG_ZYNQ_QSPI)
/* Environment in Serial Flash */
#  define CONFIG_ENV_IS_IN_SPI_FLASH
# elif defined(CONFIG_NAND_ZYNQ)
/* Environment in NAND flash */
#  define CONFIG_ENV_IS_IN_NAND
# elif defined(CONFIG_SYS_NO_FLASH)
#  define CONFIG_ENV_IS_NOWHERE
# endif

# define CONFIG_ENV_SECT_SIZE		CONFIG_ENV_SIZE
# ifndef CONFIG_ENV_OFFSET
#  define CONFIG_ENV_OFFSET		0x100000
# endif
#endif

/* enable preboot to be loaded before CONFIG_BOOTDELAY */

/* Boot configuration */

#ifdef CONFIG_SPL_BUILD
#define BOOTENV
#else

#ifdef CONFIG_CMD_MMC
#define BOOT_TARGET_DEVICES_MMC(func) func(MMC, mmc, 0) func(MMC, mmc, 1)
#else
#define BOOT_TARGET_DEVICES_MMC(func)
#endif

#ifdef CONFIG_CMD_USB
#define BOOT_TARGET_DEVICES_USB(func) func(USB, usb, 0) func(USB, usb, 1)
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

#define BOOTENV_DEV_QSPI(devtypeu, devtypel, instance) \
	"bootcmd_qspi=sf probe 0 0 0 && " \
		      "sf read ${scriptaddr} ${script_offset_f} ${script_size_f} && " \
		      "echo QSPI: Trying to boot script at ${scriptaddr} && " \
		      "source ${scriptaddr}; echo QSPI: SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_QSPI(devtypeu, devtypel, instance) \
	"qspi "

#define BOOTENV_DEV_NAND(devtypeu, devtypel, instance) \
	"bootcmd_nand=nand info && " \
		      "nand read ${scriptaddr} ${script_offset_f} ${script_size_f} && " \
		      "echo NAND: Trying to boot script at ${scriptaddr} && " \
		      "source ${scriptaddr}; echo NAND: SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_NAND(devtypeu, devtypel, instance) \
	"nand "

#define BOOTENV_DEV_NOR(devtypeu, devtypel, instance) \
	"script_offset_nor=0xE2FC0000\0"        \
	"bootcmd_nor=cp.b ${script_offset_nor} ${scriptaddr} ${script_size_f} && " \
		     "echo NOR: Trying to boot script at ${scriptaddr} && " \
		     "source ${scriptaddr}; echo NOR: SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_NOR(devtypeu, devtypel, instance) \
	"nor "

#define BOOT_TARGET_DEVICES_JTAG(func)  func(JTAG, jtag, na)

#define BOOTENV_DEV_JTAG(devtypeu, devtypel, instance) \
	"bootcmd_jtag=echo JTAG: Trying to boot script at ${scriptaddr} && " \
		"source ${scriptaddr}; echo JTAG: SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_JTAG(devtypeu, devtypel, instance) \
	"jtag "

#define BOOT_TARGET_DEVICES_USB_DFU(func) \
	func(USB_DFU, usb_dfu, 0) func(USB_DFU, usb_dfu, 1)

#define BOOTENV_DEV_USB_DFU(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=setenv dfu_alt_info boot.scr ram " \
	"$scriptaddr $script_size_f && " \
	"dfu " #instance " ram " #instance " 60 && " \
	"echo DFU" #instance ": Trying to boot script at ${scriptaddr} && " \
	"source ${scriptaddr}; " \
	"echo DFU" #instance ": SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_USB_DFU(devtypeu, devtypel, instance) \
	""

#define BOOT_TARGET_DEVICES_USB_THOR(func) \
	func(USB_THOR, usb_thor, 0) func(USB_THOR, usb_thor, 1)

#define BOOTENV_DEV_USB_THOR(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=setenv dfu_alt_info boot.scr ram " \
	"$scriptaddr $script_size_f && " \
	"thordown " #instance " ram " #instance " && " \
	"echo THOR" #instance ": Trying to boot script at ${scriptaddr} && " \
	"source ${scriptaddr}; " \
	"echo THOR" #instance ": SCRIPT FAILED: continuing...;\0"

#define BOOTENV_DEV_NAME_USB_THOR(devtypeu, devtypel, instance) \
	""

#define BOOT_TARGET_DEVICES(func) \
	BOOT_TARGET_DEVICES_JTAG(func) \
	BOOT_TARGET_DEVICES_MMC(func) \
	BOOT_TARGET_DEVICES_QSPI(func) \
	BOOT_TARGET_DEVICES_NAND(func) \
	BOOT_TARGET_DEVICES_NOR(func) \
	BOOT_TARGET_DEVICES_USB_DFU(func) \
	BOOT_TARGET_DEVICES_USB_THOR(func) \
	BOOT_TARGET_DEVICES_USB(func) \
	BOOT_TARGET_DEVICES_PXE(func) \
	BOOT_TARGET_DEVICES_DHCP(func)

#include <config_distro_bootcmd.h>
#endif /* CONFIG_SPL_BUILD */

/* Initial bootstrap JTAG u-boot
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_BOOTCOMMAND		"run jtagboot"
*/

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
	"sdboot=if mmcinfo; then " \
			"run uenvboot; " \
			"echo Starting Linux from SD... && " \
			"bootm ${fit_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
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

#define CONFIG_SYS_INIT_RAM_ADDR	0xFFFF0000
#define CONFIG_SYS_INIT_RAM_SIZE	0x2000


/* Extend size of kernel image for uncompression */

/* Address in RAM where the parameters must be copied by SPL. */

/* Not using MMC raw mode - just for compilation purpose */

/* qspi mode is working fine */
#ifdef CONFIG_ZYNQ_QSPI
#define CONFIG_SYS_SPI_ARGS_OFFS	0x200000
#define CONFIG_SYS_SPI_ARGS_SIZE	0x80000
#define CONFIG_SYS_SPI_KERNEL_OFFS	(CONFIG_SYS_SPI_ARGS_OFFS + \
					CONFIG_SYS_SPI_ARGS_SIZE)
#endif

/* SP location before relocation, must use scratch RAM */

/* 3 * 64kB blocks of OCM - one is on the top because of bootrom */

/* On the top of OCM space */

/*
 * SPL stack position - and stack goes down
 * 0xfffffe00 is used for putting wfi loop.
 * Set it up as limit for now.
 */

#endif /* __CONFIG_ZYNQ_COMMON_H */
