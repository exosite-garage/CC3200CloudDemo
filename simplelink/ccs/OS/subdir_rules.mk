################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cc_pal.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/cc_pal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="cc_pal.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

device.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/device.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="device.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/driver.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="driver.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

flowcont.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/flowcont.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="flowcont.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fs.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/fs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="fs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

netapp.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/netapp.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="netapp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

netcfg.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/netcfg.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="netcfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nonos.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/nonos.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="nonos.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

socket.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/socket.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="socket.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

spawn.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/spawn.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="spawn.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

wlan.obj: V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source/wlan.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Include" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/simplelink/Source" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/driverlib" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/inc" --include_path="V:/TI/exosite_cloud_demo_for_cc3200/oslib/" -g --gcc --define=ccs --define=SL_FULL --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --ual --preproc_with_compile --preproc_dependency="wlan.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


