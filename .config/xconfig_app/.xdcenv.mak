#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_45_01_29/packages;C:/ti/edma3_lld_02_12_00_20/packages;C:/ti/ccsv6/ccs_base;C:/Users/tupers/workspace_v6_0/edma_test/.config
override XDCROOT = C:/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_45_01_29/packages;C:/ti/edma3_lld_02_12_00_20/packages;C:/ti/ccsv6/ccs_base;C:/Users/tupers/workspace_v6_0/edma_test/.config;C:/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
