################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

C_SRCS += \
../blockAlg.c \
../edma.c \
../main.c \
../sample_c6657_cfg.c 

OBJS += \
./blockAlg.obj \
./edma.obj \
./main.obj \
./sample_c6657_cfg.obj 

C_DEPS += \
./blockAlg.pp \
./edma.pp \
./main.pp \
./sample_c6657_cfg.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"blockAlg.pp" \
"edma.pp" \
"main.pp" \
"sample_c6657_cfg.pp" 

OBJS__QUOTED += \
"blockAlg.obj" \
"edma.obj" \
"main.obj" \
"sample_c6657_cfg.obj" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 

C_SRCS__QUOTED += \
"../blockAlg.c" \
"../edma.c" \
"../main.c" \
"../sample_c6657_cfg.c" 


