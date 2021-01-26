################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AlignmentFunctions.cpp \
../ComponentEmptyException.cpp \
../DNAfunctions.cpp \
../FastaSequence.cpp \
../FastqException.cpp \
../FastqSequence.cpp \
../PhredScoreFunctions.cpp \
../QualityNotEqualSequenceException.cpp 

OBJS += \
./AlignmentFunctions.o \
./ComponentEmptyException.o \
./DNAfunctions.o \
./FastaSequence.o \
./FastqException.o \
./FastqSequence.o \
./PhredScoreFunctions.o \
./QualityNotEqualSequenceException.o 

CPP_DEPS += \
./AlignmentFunctions.d \
./ComponentEmptyException.d \
./DNAfunctions.d \
./FastaSequence.d \
./FastqException.d \
./FastqSequence.d \
./PhredScoreFunctions.d \
./QualityNotEqualSequenceException.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -std=c++0x -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


