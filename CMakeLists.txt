cmake_minimum_required(VERSION 2.6)

project(UsbDevice_With_Library)

# Probe the environment (OS + architecture)
include(${CMAKE_CURRENT_SOURCE_DIR}/env_probe.cmake)

# Add include directory
set(PRJ_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



# if(WIN32)
    #Add library directory 
    set(PRJ_LIBRARIES_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/${PLATFORM}
    )   
    link_directories(${PRJ_LIBRARIES_DIR})
# endif()

if(UNIX)
    # Set compiler flag
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lpthread -s -Wall -std=c++14 -fexceptions -O2")
endif(UNIX)

# reference: https://stackoverflow.com/questions/34995936/how-to-link-a-third-party-library-libusb-in-cmake
# libusb.lib
find_library(LIBUSB_LIBRARIES
    NAMES 
        libusb libusb-1.0 usb-1.0
    PATHS
        ${PRJ_LIBRARIES_DIR}
)

# Display information
# message("CMAKE_CURRENT_SOURCE_DIR    : ${CMAKE_CURRENT_SOURCE_DIR}")
message("**********************************************")
message("Target platform    : ${PLATFORM}")
if(CMAKE_CONFIGURATION_TYPES)
    message("Multiconfig        : ${CMAKE_CONFIGURATION_TYPES}")
else()
    message("Build type         : ${CMAKE_BUILD_TYPE}")
endif()
message("Include dir        : ${PRJ_INCLUDE_DIR}")
message("Libraries dir      : ${PRJ_LIBRARIES_DIR}")
message("Libraries          : ${LIBUSB_LIBRARIES}")
message("**********************************************")


######################### SAMPLE PROJECTS #########################
# hotplug_linux
add_executable(hotplug_linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hotplug_linux.cpp
)
target_link_libraries(hotplug_linux ${LIBUSB_LIBRARIES})

#devlist
add_executable(devlist
    ${CMAKE_CURRENT_SOURCE_DIR}/src/devlist.cpp
)
target_link_libraries(devlist ${LIBUSB_LIBRARIES})