cmake_minimum_required(VERSION 3.5.0)
project(reprotobuf VERSION 0.1.0 LANGUAGES C CXX)


set(SOFTWARE_DETAILS
    res/app.rc
)


set(HEADERS
    src/core/context.h
    src/version.h
)


set(SOURCES
    src/core/context.cpp
    src/main.cpp
)


include_directories(src)


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SOFTWARE_DETAILS})


# ตั้งค่าสภาพแวดล้อมในการ build เพื่อเอาไปใช้งาน code ต่อ (_DEBUG)
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message("Configuring Debug build")
    add_definitions(-D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    message("Configuring Release build")
endif()