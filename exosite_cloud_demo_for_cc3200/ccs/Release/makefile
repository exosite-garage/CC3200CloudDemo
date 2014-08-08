################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/arm_5.1.5

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./exosite/exosite_meta.obj" \
"./exosite/exosite_hal.obj" \
"./exosite/exosite.obj" \
"./utilities/strlib.obj" \
"./uart_if.obj" \
"./tmp006drv.obj" \
"./timer_if.obj" \
"./main.obj" \
"./i2c_if.obj" \
"./gpio_if.obj" \
"./cloud_demo.obj" \
"./button_if.obj" \
"./bma222drv.obj" \
"../cc3200v1p32.cmd" \
-l"libc.a" \
-l"V:/TI/exosite_cloud_demo_for_cc3200/exosite_cloud_demo_for_cc3200/ccs\..\..\oslib\ccs\ti_rtos\ti_rtos.a" \
-l"V:/TI/exosite_cloud_demo_for_cc3200/exosite_cloud_demo_for_cc3200/ccs\..\..\driverlib\ccs\Release\driverlib.a" \
-l"V:/TI/exosite_cloud_demo_for_cc3200/exosite_cloud_demo_for_cc3200/ccs\..\..\simplelink\ccs\OS\simplelink.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include utilities/subdir_vars.mk
-include exosite/subdir_vars.mk
-include subdir_rules.mk
-include utilities/subdir_rules.mk
-include exosite/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
exosite_cloud_demo_for_cc3200.out \

EXE_OUTPUTS__QUOTED += \
"exosite_cloud_demo_for_cc3200.out" \

BIN_OUTPUTS += \
exosite_cloud_demo_for_cc3200.hex \

BIN_OUTPUTS__QUOTED += \
"exosite_cloud_demo_for_cc3200.hex" \


# All Target
all: exosite_cloud_demo_for_cc3200.out

dependents:
	-cd C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default && $(MAKE) clean all

# Tool invocations
exosite_cloud_demo_for_cc3200.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/linker.cmd
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -g --gcc --define=ccs --define=NOTERM --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off -z -m"exosite_cloud_demo_for_cc3200.map" --heap_size=0x4000 --stack_size=0x4000 -i"c:/ti/ccsv6/tools/compiler/arm_5.1.5/lib" -i"c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="exosite_cloud_demo_for_cc3200_linkInfo.xml" --rom_model -l"C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/linker.cmd" -o "exosite_cloud_demo_for_cc3200.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

exosite_cloud_demo_for_cc3200.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armhex"  -o "exosite_cloud_demo_for_cc3200.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "bma222drv.pp" "button_if.pp" "cloud_demo.pp" "gpio_if.pp" "i2c_if.pp" "main.pp" "timer_if.pp" "tmp006drv.pp" "uart_if.pp" "utilities\strlib.pp" "exosite\exosite.pp" "exosite\exosite_hal.pp" "exosite\exosite_meta.pp" 
	-$(RM) "bma222drv.obj" "button_if.obj" "cloud_demo.obj" "gpio_if.obj" "i2c_if.obj" "main.obj" "timer_if.obj" "tmp006drv.obj" "uart_if.obj" "utilities\strlib.obj" "exosite\exosite.obj" "exosite\exosite_hal.obj" "exosite\exosite_meta.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-@echo 'convert to bin'
	-"c:/ti/ccsv6/utils/tiobj2bin/tiobj2bin" "exosite_cloud_demo_for_cc3200.out" "exosite_cloud_demo_for_cc3200.bin" "c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armofd" "c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armhex" "c:/ti/ccsv6/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/linker.cmd:

-include ../makefile.targets

