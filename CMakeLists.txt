cmake_minimum_required(VERSION 3.5.0)
project(reprotobuf VERSION 0.1.0 LANGUAGES C CXX)

# ใช้ Windows PowerShell แทน PowerShell Core
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_SYSTEM_PROGRAM pwsh "powershell.exe")


set(SOFTWARE_DETAILS
    res/app.rc
)


set(HEADERS
    
)


set(SOURCES
    src/main.cpp
)


include_directories(src)


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SOFTWARE_DETAILS})


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Configuring Debug build")
    add_definitions(-D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Configuring Release build")
endif()