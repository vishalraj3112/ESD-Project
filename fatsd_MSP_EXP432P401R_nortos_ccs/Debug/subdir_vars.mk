################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_NoRTOS.cmd 

SYSCFG_SRCS += \
../fatsd.syscfg 

C_SRCS += \
./syscfg/ti_drivers_config.c \
../fatsd_nortos.c \
../lcd.c \
../main_nortos.c \
../mpu.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/ti_drivers_config.d \
./fatsd_nortos.d \
./lcd.d \
./main_nortos.d \
./mpu.d 

OBJS += \
./syscfg/ti_drivers_config.obj \
./fatsd_nortos.obj \
./lcd.obj \
./main_nortos.obj \
./mpu.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\ti_drivers_config.obj" \
"fatsd_nortos.obj" \
"lcd.obj" \
"main_nortos.obj" \
"mpu.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"syscfg\ti_drivers_config.d" \
"fatsd_nortos.d" \
"lcd.d" \
"main_nortos.d" \
"mpu.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../fatsd.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_drivers_config.c" \
"../fatsd_nortos.c" \
"../lcd.c" \
"../main_nortos.c" \
"../mpu.c" 


