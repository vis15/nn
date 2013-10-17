################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../config.cpp \
../log.cpp \
../main.cpp \
../mf.cpp \
../utils.cpp \
../vars.cpp 

OBJS += \
./config.o \
./log.o \
./main.o \
./mf.o \
./utils.o \
./vars.o 

CPP_DEPS += \
./config.d \
./log.d \
./main.d \
./mf.d \
./utils.d \
./vars.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


