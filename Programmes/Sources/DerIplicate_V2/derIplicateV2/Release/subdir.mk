################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp \
../SimpleFastaSequence.cpp 

OBJS += \
./Main.o \
./SimpleFastaSequence.o 

CPP_DEPS += \
./Main.d \
./SimpleFastaSequence.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -I"SOURCE_FOLDER_HERE/DerIplicate_V2/tclaplib/include" -I"SOURCE_FOLDER_HERE/DerIplicate_V2/DNAlibPG" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


