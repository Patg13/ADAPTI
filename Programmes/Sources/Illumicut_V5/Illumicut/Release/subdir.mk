################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp \
../NewAlignmentFcts.cpp \
../NewDNAFunctions.cpp \
../Repartition.cpp \
../SimpleFastaSequence.cpp 

OBJS += \
./Main.o \
./NewAlignmentFcts.o \
./NewDNAFunctions.o \
./Repartition.o \
./SimpleFastaSequence.o 

CPP_DEPS += \
./Main.d \
./NewAlignmentFcts.d \
./NewDNAFunctions.d \
./Repartition.d \
./SimpleFastaSequence.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -I"SOURCE_FOLDER_HERE/Illumicut_V5/tclaplib/include" -I"SOURCE_FOLDER_HERE/Illumicut_V5/DNAlibPG" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


