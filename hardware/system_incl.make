#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:C:\RobotRacers\CultOfSkaro\hardware\system.xmp
#
# WARNING : This file will be re-generated every time a command
# to run a make target is invoked. So, any changes made to this  
# file manually, will be lost when make is invoked next. 
#################################################################

SHELL = CMD

XILINX_EDK_DIR = C:/Xilinx/13.3/ISE_DS/EDK

SYSTEM = system

MHSFILE = system.mhs

FPGA_ARCH = virtex4

DEVICE = xc4vfx60ff672-11

INTSTYLE = default

LANGUAGE = vhdl
GLOBAL_SEARCHPATHOPT = 
PROJECT_SEARCHPATHOPT = 

SEARCHPATHOPT = $(PROJECT_SEARCHPATHOPT) $(GLOBAL_SEARCHPATHOPT)

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) -intstyle $(INTSTYLE) $(SEARCHPATHOPT) $(SUBMODULE_OPT) -msg __xps/ise/xmsgprops.lst

OBSERVE_PAR_OPTIONS = -error yes

MICROBLAZE_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop.elf
MICROBLAZE_BOOTLOOP_LE = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop_le.elf
PPC405_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc405/ppc_bootloop.elf
PPC440_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc440/ppc440_bootloop.elf
BOOTLOOP_DIR = bootloops

PPC405_0_BOOTLOOP = $(BOOTLOOP_DIR)/ppc405_0.elf

PPC405_1_BOOTLOOP = $(BOOTLOOP_DIR)/ppc405_1.elf

BRAMINIT_ELF_IMP_FILES = $(PPC405_0_BOOTLOOP) $(PPC405_1_BOOTLOOP)
BRAMINIT_ELF_IMP_FILE_ARGS = -pe ppc405_0 $(PPC405_0_BOOTLOOP) -pe ppc405_1 $(PPC405_1_BOOTLOOP)

BRAMINIT_ELF_SIM_FILES = $(PPC405_0_BOOTLOOP) $(PPC405_1_BOOTLOOP)
BRAMINIT_ELF_SIM_FILE_ARGS = -pe ppc405_0 $(PPC405_0_BOOTLOOP) -pe ppc405_1 $(PPC405_1_BOOTLOOP)

SIM_CMD = isim_system

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM)_setup.tcl

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM)_setup.tcl

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM)_setup.tcl

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) -intstyle $(INTSTYLE) $(SEARCHPATHOPT) $(BRAMINIT_ELF_SIM_FILE_ARGS) -msg __xps/ise/xmsgprops.lst -s isim -tb


CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/ppc405_0_wrapper.ngc \
implementation/ppc405_1_wrapper.ngc \
implementation/plb_0_wrapper.ngc \
implementation/plb_1_wrapper.ngc \
implementation/xps_bram_if_cntlr_0_wrapper.ngc \
implementation/xps_bram_if_cntlr_1_wrapper.ngc \
implementation/plb_bram_if_cntlr_0_bram_wrapper.ngc \
implementation/plb_bram_if_cntlr_1_bram_wrapper.ngc \
implementation/serial_uart_wrapper.ngc \
implementation/wireless_uart_wrapper.ngc \
implementation/clock_generator_0_wrapper.ngc \
implementation/jtagppc_cntlr_inst_wrapper.ngc \
implementation/proc_sys_reset_0_wrapper.ngc \
implementation/xps_intc_0_wrapper.ngc \
implementation/plb_quad_encoder_0_wrapper.ngc \
implementation/plb_pwm_ctrl_0_wrapper.ngc \
implementation/xps_mch_emc_flash_wrapper.ngc \
implementation/xps_mch_emc_sram_wrapper.ngc \
implementation/dcm_module_0_wrapper.ngc \
implementation/plb_usb_0_wrapper.ngc \
implementation/plb_vision_0_wrapper.ngc \
implementation/mpmc_0_wrapper.ngc \
implementation/dcm_2_wrapper.ngc \
implementation/xps_gpio_1_wrapper.ngc \
implementation/debug_module_0_wrapper.ngc \
implementation/gameboard_uart_wrapper.ngc \
implementation/xps_intc_1_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data\system.ucf

BMM_FILE = implementation/$(SYSTEM).bmm

BITGEN_UT_FILE = etc/bitgen.ut

XFLOW_OPT_FILE = etc/fast_runtime.opt
XFLOW_DEPENDENCY = __xps/xpsxflow.opt $(XFLOW_OPT_FILE)

XPLORER_DEPENDENCY = __xps/xplorer.opt
XPLORER_OPTIONS = -p $(DEVICE) -uc $(SYSTEM).ucf -bm $(SYSTEM).bmm -max_runs 7

FPGA_IMP_DEPENDENCY = $(BMM_FILE) $(POSTSYN_NETLIST) $(UCF_FILE) $(XFLOW_DEPENDENCY)

SDK_EXPORT_DIR = SDK\SDK_Export\hw
SYSTEM_HW_HANDOFF = $(SDK_EXPORT_DIR)/$(SYSTEM).xml
SYSTEM_HW_HANDOFF_BIT = $(SDK_EXPORT_DIR)/$(SYSTEM).bit
SYSTEM_HW_HANDOFF_BMM = $(SDK_EXPORT_DIR)/$(SYSTEM)_bd.bmm
SYSTEM_HW_HANDOFF_DEP = $(SYSTEM_HW_HANDOFF) $(SYSTEM_HW_HANDOFF_BIT) $(SYSTEM_HW_HANDOFF_BMM)