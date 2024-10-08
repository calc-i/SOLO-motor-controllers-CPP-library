cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(SOLOmotorcontrollersCPPlibrary VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define configurations (Release and Debug)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Source files
set(SOURCE_FILES
    src/SOLOMotorControllers.cpp
    src/SOLOMotorControllersSerial.cpp
    src/SOLOMotorControllersUtils.cpp
)

# Header files
set(HEADER_FILES
    inc/canevt.h
    inc/canlib.h
    inc/canstat.h
    inc/j1587lib.h
    inc/kvaDbLib.h
    inc/kvaMemoLibXML.h
    inc/kvaser_stdint.h
    inc/kvDiag.h
    inc/kvlclib.h
    inc/kvmlib.h
    inc/kvrlib.h
    inc/linlib.h
    inc/obsolete.h
    inc/predef.h
    inc/sing.h
    inc/std.h
    src/Kvaser.h
    src/SOLOMotorControllers.h
    src/SOLOMotorControllersCanopenKvaser.h
    src/SOLOMotorControllersSerial.h
    src/SOLOMotorControllersUtils.h
)

# Create a static or shared library
# You can choose STATIC or SHARED depending on whether you want to create a static or shared library.
add_library(SOLOMotorControllers STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Set the include directories
target_include_directories(SOLOMotorControllers PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/src
)

# Link the external library (canlib32.lib)
target_link_directories(SOLOMotorControllers PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(SOLOMotorControllers PRIVATE canlib32)

# Set platform-specific options
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_ARCH "x64")
else()
    set(PLATFORM_ARCH "Win32")
endif()

# Set flags for different configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(SOLOMotorControllers PRIVATE _DEBUG _CONSOLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MTd")
else()
    target_compile_definitions(SOLOMotorControllers PRIVATE NDEBUG _CONSOLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /O2 /MT")
endif()

# Define additional compiler options
target_compile_options(SOLOMotorControllers PRIVATE
    /SDL
    /permissive-    # Conformance Mode
)

# Set the character set (Unicode for Win32 and MultiByte for x64)
if(PLATFORM_ARCH STREQUAL "x64")
    target_compile_definitions(SOLOMotorControllers PRIVATE _MBCS)
else()
    target_compile_definitions(SOLOMotorControllers PRIVATE UNICODE)
endif()
