################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
utilities/strlib.obj: V:/TI/cc3200_cloud/exosite_cloud_demo_for_cc3200/utilities/strlib.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/common" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/exosite_cloud_demo_for_cc3200/utilities" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/exosite_cloud_demo_for_cc3200/exosite" -g --gcc --define=ccs --define=NOTERM --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="utilities/strlib.pp" --obj_directory="utilities" --cmd_file="C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


