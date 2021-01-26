################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp \
../NewDNAFunctions.cpp \
../Repartition.cpp \
../SimpleFastaSequence.cpp 

OBJS += \
./Main.o \
./NewDNAFunctions.o \
./Repartition.o \
./SimpleFastaSequence.o 

CPP_DEPS += \
./Main.d \
./NewDNAFunctions.d \
./Repartition.d \
./SimpleFastaSequence.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -std=c++0x -std=c++11 -I/home/patrick/Gouvernement/Current/PatFinder_release/tclap-1.2.1/include -I"/home/patrick/Gouvernement/Current/PatFinder_release/DNAlibPG" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


