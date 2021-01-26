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
	g++ -std=c++0x -I"SOURCE_FOLDER_HERE/PatFinder_V2/DNAlibPG" -I"SOURCE_FOLDER_HERE/PatFinder_V2/tclap-1.2.1/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


