################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../examples/test1.cpp \
../examples/test10.cpp \
../examples/test11.cpp \
../examples/test12.cpp \
../examples/test13.cpp \
../examples/test14.cpp \
../examples/test15.cpp \
../examples/test16.cpp \
../examples/test17-a.cpp \
../examples/test17.cpp \
../examples/test18.cpp \
../examples/test19.cpp \
../examples/test2.cpp \
../examples/test20.cpp \
../examples/test21.cpp \
../examples/test3.cpp \
../examples/test4.cpp \
../examples/test5.cpp \
../examples/test6.cpp \
../examples/test7.cpp \
../examples/test8.cpp \
../examples/test9.cpp 

OBJS += \
./examples/test1.o \
./examples/test10.o \
./examples/test11.o \
./examples/test12.o \
./examples/test13.o \
./examples/test14.o \
./examples/test15.o \
./examples/test16.o \
./examples/test17-a.o \
./examples/test17.o \
./examples/test18.o \
./examples/test19.o \
./examples/test2.o \
./examples/test20.o \
./examples/test21.o \
./examples/test3.o \
./examples/test4.o \
./examples/test5.o \
./examples/test6.o \
./examples/test7.o \
./examples/test8.o \
./examples/test9.o 

CPP_DEPS += \
./examples/test1.d \
./examples/test10.d \
./examples/test11.d \
./examples/test12.d \
./examples/test13.d \
./examples/test14.d \
./examples/test15.d \
./examples/test16.d \
./examples/test17-a.d \
./examples/test17.d \
./examples/test18.d \
./examples/test19.d \
./examples/test2.d \
./examples/test20.d \
./examples/test21.d \
./examples/test3.d \
./examples/test4.d \
./examples/test5.d \
./examples/test6.d \
./examples/test7.d \
./examples/test8.d \
./examples/test9.d 


# Each subdirectory must supply rules for building sources it contributes
examples/%.o: ../examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


