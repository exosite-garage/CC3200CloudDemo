################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
osi_tirtos.obj: C:/Users/Patrick/Desktop/CC3200CloudDemo-master/oslib/osi_tirtos.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me -Ooff --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/oslib" -g --gcc --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="osi_tirtos.pp" --cmd_file="C:/ti/tirtos_simplelink_2_00_02_36/products/cc3200-sdk_0.51a/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


