#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_45_01_29/packages;C:/ti/edma3_lld_02_12_00_20/packages;C:/ti/framework_components_3_40_02_07/packages;C:/ti/framework_components_3_40_02_07/examples;C:/ti/framework_components_3_40_02_07/fctools/packages;C:/ti/uia_2_00_03_43/packages;C:/ti/ccsv6/ccs_base
override XDCROOT = C:/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_45_01_29/packages;C:/ti/edma3_lld_02_12_00_20/packages;C:/ti/framework_components_3_40_02_07/packages;C:/ti/framework_components_3_40_02_07/examples;C:/ti/framework_components_3_40_02_07/fctools/packages;C:/ti/uia_2_00_03_43/packages;C:/ti/ccsv6/ccs_base;C:/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
