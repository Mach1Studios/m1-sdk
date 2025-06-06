# Mach1 Spatial SDK
# Compile and Mach1Transcode API & Mach1Decode API audioplayer example

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

build:
ifeq ($(detected_OS),Windows)
	cmake . -Bbuild -A x64
	cmake --build build
else
	cmake . -Bbuild
	cmake --build build
endif
