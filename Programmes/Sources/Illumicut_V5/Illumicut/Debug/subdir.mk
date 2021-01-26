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
	g++ -std=gnu++11 -std=c++0x -std=c++11 -I"/data/home2/pagagne/Developpement/Illumicut_V5/tclaplib/include" -I"/data/home2/pagagne/Developpement/Illumicut_V5/DNAlibPG" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


