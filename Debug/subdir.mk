################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../input_matrix.c \
../leading_eigenpair.c \
../main.c \
../modularity_matrix.c \
../power_iteration.c 

O_SRCS += \
../cluster.o \
../input_matrix.o \
../main.o \
../modularity_matrix.o 

OBJS += \
./input_matrix.o \
./leading_eigenpair.o \
./main.o \
./modularity_matrix.o \
./power_iteration.o 

C_DEPS += \
./input_matrix.d \
./leading_eigenpair.d \
./main.d \
./modularity_matrix.d \
./power_iteration.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


