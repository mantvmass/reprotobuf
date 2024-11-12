TARGET := build/
BUILD_TYPE ?= RELEASE  # ค่าเริ่มต้นเป็น RELEASE | ค่านี้จะถูกนำไปใช้งานต่อใน CMakeLists.txt

# ตั้งค่าตัวแปร BUILD_TYPE เป็น DEBUG ถ้าเรียกใช้ make debug
ifeq ($(MAKECMDGOALS), debug)
    BUILD_TYPE := DEBUG
endif

all:
	cmake -S . -B $(TARGET) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

debug: all
	cmake --build $(TARGET) --config Debug -- /p:VcpkgEnableManifest=true

release: all
	cmake --build $(TARGET) --config Release -- /p:VcpkgEnableManifest=true