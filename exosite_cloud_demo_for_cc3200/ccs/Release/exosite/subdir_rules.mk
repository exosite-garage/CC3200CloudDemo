################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
exosite/exosite.obj: C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite/exosite.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/oslib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Include" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Source" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/inc" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/driverlib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/common" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/utilities" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite" -g --gcc --define=ccs --define=NOTERM --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="exosite/exosite.pp" --obj_directory="exosite" --cmd_file="C:/ti/tirtos_simplelink_2_00_02_36/products/cc3200-sdk_0.51a/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

exosite/exosite_hal.obj: C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite/exosite_hal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/oslib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Include" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Source" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/inc" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/driverlib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/common" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/utilities" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite" -g --gcc --define=ccs --define=NOTERM --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="exosite/exosite_hal.pp" --obj_directory="exosite" --cmd_file="C:/ti/tirtos_simplelink_2_00_02_36/products/cc3200-sdk_0.51a/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

exosite/exosite_meta.obj: C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite/exosite_meta.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/oslib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Include" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/simplelink/Source" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/inc" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/driverlib" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/common" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/utilities" --include_path="C:/Users/Patrick/Desktop/CC3200CloudDemo-master/exosite_cloud_demo_for_cc3200/exosite" -g --gcc --define=ccs --define=NOTERM --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="exosite/exosite_meta.pp" --obj_directory="exosite" --cmd_file="C:/ti/tirtos_simplelink_2_00_02_36/products/cc3200-sdk_0.51a/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


