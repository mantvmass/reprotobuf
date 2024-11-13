cmake_minimum_required(VERSION 3.5.0)
project(reprotobuf VERSION 0.1.0 LANGUAGES C CXX)


# กำหนดมาตรฐาน C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# ตั้งค่าให้รองรับ Unicode แทน ASCII สำหรับการจัดการกับสตริงในโปรแกรม Win32 API สำหรับ Visual Studio หรือ MSVC
if(MSVC)
    add_compile_options("/DUNICODE" "/D_UNICODE") # อย่าลืมใช้ L หน้าตัวแปรข้อความ (L"ตัวแปร") เพื่อเก็บข้อมูลแบบ wchar_t นะจ๊ะ
endif()


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


# รวมไฟล์จาก UI components ด้วย Glob
file(GLOB UI_SOURCES src/components/*.cpp)
file(GLOB UI_HEADERS src/components/*.h)


# กำหนดไดเรกทอรีที่เก็บไฟล์ sources
include_directories(src)

# สร้าง executable
add_executable(
    ${PROJECT_NAME}
    WIN32
    ${HEADERS}
    ${SOURCES}
    ${SOFTWARE_DETAILS}
    ${UI_HEADERS}
    ${UI_SOURCES}
)

# ตั้งค่าสภาพแวดล้อมในการ build เพื่อเอาไปใช้งานใน code ต่อ (_DEBUG)
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message("Configuring Debug build")
    add_definitions(-D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    message("Configuring Release build")
endif()

# ลิงค์ไลบรารี
target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32 kernel32)