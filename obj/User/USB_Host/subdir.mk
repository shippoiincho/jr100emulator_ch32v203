################################################################################
# MRS Version: {"version":"1.8.5","date":"2023/05/22"}
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/USB_Host/app_km.c \
../User/USB_Host/ch32v20x_usbfs_host.c \
../User/USB_Host/usb_host_hid.c \
../User/USB_Host/usb_host_hub.c 

OBJS += \
./User/USB_Host/app_km.o \
./User/USB_Host/ch32v20x_usbfs_host.o \
./User/USB_Host/usb_host_hid.o \
./User/USB_Host/usb_host_hub.o 

C_DEPS += \
./User/USB_Host/app_km.d \
./User/USB_Host/ch32v20x_usbfs_host.d \
./User/USB_Host/usb_host_hid.d \
./User/USB_Host/usb_host_hub.d 


# Each subdirectory must supply rules for building sources it contributes
User/USB_Host/%.o: ../User/USB_Host/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\MRS_DATA\workspace\jr100c_CH32V203\Debug" -I"C:\MRS_DATA\workspace\jr100c_CH32V203\User\USB_Host" -I"C:\MRS_DATA\workspace\jr100c_CH32V203\Core" -I"C:\MRS_DATA\workspace\jr100c_CH32V203\User" -I"C:\MRS_DATA\workspace\jr100c_CH32V203\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

