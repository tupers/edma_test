################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

C_SRCS += \
../blockQueue.c \
../block_alg.c \
../edmaTest.c \
../main.c \
../sample_c6657_cfg.c 

OBJS += \
./blockQueue.obj \
./block_alg.obj \
./edmaTest.obj \
./main.obj \
./sample_c6657_cfg.obj 

C_DEPS += \
./blockQueue.pp \
./block_alg.pp \
./edmaTest.pp \
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
"blockQueue.pp" \
"block_alg.pp" \
"edmaTest.pp" \
"main.pp" \
"sample_c6657_cfg.pp" 

OBJS__QUOTED += \
"blockQueue.obj" \
"block_alg.obj" \
"edmaTest.obj" \
"main.obj" \
"sample_c6657_cfg.obj" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 

C_SRCS__QUOTED += \
"../blockQueue.c" \
"../block_alg.c" \
"../edmaTest.c" \
"../main.c" \
"../sample_c6657_cfg.c" 


