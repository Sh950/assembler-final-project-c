################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../assembler.c \
../decoding.c \
../firstPass.c \
../macro.c \
../secondPass.c \
../symbolTable.c \
../utils.c 

OBJS += \
./assembler.o \
./decoding.o \
./firstPass.o \
./macro.o \
./secondPass.o \
./symbolTable.o \
./utils.o 

C_DEPS += \
./assembler.d \
./decoding.d \
./firstPass.d \
./macro.d \
./secondPass.d \
./symbolTable.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


