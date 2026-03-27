.PHONY: all configure build run clean

all: build

configure:
	cmake -B build

build: configure
	cmake --build build

run: build
	./build/AlgoTrace

clean:
	rm -rf build