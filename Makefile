TARGET := build/

all:
	cmake -S . -B ${TARGET}

debug: all
	cmake --build ${TARGET} --config Debug

release: all
	cmake --build ${TARGET} --config Release