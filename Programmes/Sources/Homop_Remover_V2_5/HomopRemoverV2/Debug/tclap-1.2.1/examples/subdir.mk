################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tclap-1.2.1/examples/test1.cpp \
../tclap-1.2.1/examples/test10.cpp \
../tclap-1.2.1/examples/test11.cpp \
../tclap-1.2.1/examples/test12.cpp \
../tclap-1.2.1/examples/test13.cpp \
../tclap-1.2.1/examples/test14.cpp \
../tclap-1.2.1/examples/test15.cpp \
../tclap-1.2.1/examples/test16.cpp \
../tclap-1.2.1/examples/test17-a.cpp \
../tclap-1.2.1/examples/test17.cpp \
../tclap-1.2.1/examples/test18.cpp \
../tclap-1.2.1/examples/test19.cpp \
../tclap-1.2.1/examples/test2.cpp \
../tclap-1.2.1/examples/test20.cpp \
../tclap-1.2.1/examples/test21.cpp \
../tclap-1.2.1/examples/test3.cpp \
../tclap-1.2.1/examples/test4.cpp \
../tclap-1.2.1/examples/test5.cpp \
../tclap-1.2.1/examples/test6.cpp \
../tclap-1.2.1/examples/test7.cpp \
../tclap-1.2.1/examples/test8.cpp \
../tclap-1.2.1/examples/test9.cpp 

OBJS += \
./tclap-1.2.1/examples/test1.o \
./tclap-1.2.1/examples/test10.o \
./tclap-1.2.1/examples/test11.o \
./tclap-1.2.1/examples/test12.o \
./tclap-1.2.1/examples/test13.o \
./tclap-1.2.1/examples/test14.o \
./tclap-1.2.1/examples/test15.o \
./tclap-1.2.1/examples/test16.o \
./tclap-1.2.1/examples/test17-a.o \
./tclap-1.2.1/examples/test17.o \
./tclap-1.2.1/examples/test18.o \
./tclap-1.2.1/examples/test19.o \
./tclap-1.2.1/examples/test2.o \
./tclap-1.2.1/examples/test20.o \
./tclap-1.2.1/examples/test21.o \
./tclap-1.2.1/examples/test3.o \
./tclap-1.2.1/examples/test4.o \
./tclap-1.2.1/examples/test5.o \
./tclap-1.2.1/examples/test6.o \
./tclap-1.2.1/examples/test7.o \
./tclap-1.2.1/examples/test8.o \
./tclap-1.2.1/examples/test9.o 

CPP_DEPS += \
./tclap-1.2.1/examples/test1.d \
./tclap-1.2.1/examples/test10.d \
./tclap-1.2.1/examples/test11.d \
./tclap-1.2.1/examples/test12.d \
./tclap-1.2.1/examples/test13.d \
./tclap-1.2.1/examples/test14.d \
./tclap-1.2.1/examples/test15.d \
./tclap-1.2.1/examples/test16.d \
./tclap-1.2.1/examples/test17-a.d \
./tclap-1.2.1/examples/test17.d \
./tclap-1.2.1/examples/test18.d \
./tclap-1.2.1/examples/test19.d \
./tclap-1.2.1/examples/test2.d \
./tclap-1.2.1/examples/test20.d \
./tclap-1.2.1/examples/test21.d \
./tclap-1.2.1/examples/test3.d \
./tclap-1.2.1/examples/test4.d \
./tclap-1.2.1/examples/test5.d \
./tclap-1.2.1/examples/test6.d \
./tclap-1.2.1/examples/test7.d \
./tclap-1.2.1/examples/test8.d \
./tclap-1.2.1/examples/test9.d 


# Each subdirectory must supply rules for building sources it contributes
tclap-1.2.1/examples/%.o: ../tclap-1.2.1/examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -I"/home/NRN/pagagne/Workspaces/Homop_Remover_V2/HomopRemoverV2/tclap-1.2.1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


