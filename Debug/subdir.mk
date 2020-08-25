################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../division.c \
../input_matrix.c \
../leading_eigenpair.c \
../main.c \
../main_neo_tester.c \
../maximize_modularity.c \
../modularity_matrix.c \
../power_iteration.c \
../utils.c 

O_SRCS += \
../division.o \
../input_matrix.o \
../leading_eigenpair.o \
../main.o \
../modularity_matrix.o \
../power_iteration.o \
../utils.o 

OBJS += \
./division.o \
./input_matrix.o \
./leading_eigenpair.o \
./main.o \
./main_neo_tester.o \
./maximize_modularity.o \
./modularity_matrix.o \
./power_iteration.o \
./utils.o 

C_DEPS += \
./division.d \
./input_matrix.d \
./leading_eigenpair.d \
./main.d \
./main_neo_tester.d \
./maximize_modularity.d \
./modularity_matrix.d \
./power_iteration.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


