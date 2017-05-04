################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/%.o: ../FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4/%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-4_9-2015q3/arm-none-eabi/include" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source/include" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source/portable" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source/portable/GCC" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source/portable/GCC/ARM_CM4" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3/FRT_Library/FreeRTOS/Source/portable/MemMang" -I"C:/Users/Fsned/Documents/workspace_v7/Projekt_3" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MD -std=c99 -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


