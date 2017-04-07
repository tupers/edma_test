################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_32_00_06_core/xs" --xdcpath="C:/ti/bios_6_45_01_29/packages;C:/ti/edma3_lld_02_12_00_20/packages;C:/ti/framework_components_3_40_02_07/packages;C:/ti/framework_components_3_40_02_07/examples;C:/ti/framework_components_3_40_02_07/fctools/packages;C:/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C66 -p ti.platforms.evm6657 -r release -c "C:/ti/ccsv6/tools/compiler/c6000_7.4.12" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd

blockQueue.obj: ../blockQueue.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --include_path="C:/ti/edma3_lld_2_12_01_23/packages" --define=ADD_IRES_RESOURCE --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="blockQueue.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

block_alg.obj: ../block_alg.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --include_path="C:/ti/edma3_lld_2_12_01_23/packages" --define=ADD_IRES_RESOURCE --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="block_alg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

edmaTest.obj: ../edmaTest.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --include_path="C:/ti/edma3_lld_2_12_01_23/packages" --define=ADD_IRES_RESOURCE --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="edmaTest.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --include_path="C:/ti/edma3_lld_2_12_01_23/packages" --define=ADD_IRES_RESOURCE --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sample_c6657_cfg.obj: ../sample_c6657_cfg.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --include_path="C:/ti/edma3_lld_2_12_01_23/packages" --define=ADD_IRES_RESOURCE --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="sample_c6657_cfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


