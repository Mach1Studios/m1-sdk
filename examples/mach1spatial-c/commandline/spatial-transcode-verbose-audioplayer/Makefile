# Mach1 Spatial SDK
# Compile and Mach1Transcode API & Mach1Decode API audioplayer example

build:
ifeq ($(OS),Windows_NT)
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64) #default
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64) #amd64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86) # 32bit
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/linux/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/linux/include -L../../../../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
	ifeq ($(UNAME_S),Darwin)
		g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/xcode/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/xcode/include -L../../../../mach1spatial-libs/xcode/lib -lstdc++ -lpthread -lMach1TranscodeCAPI
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/linux/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/linux/include -L../../../../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/linux-arm/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/linux-arm/include -L../../../../mach1spatial-libs/arm-chipsets/linux-arm/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
endif

build-beaglebone:
	g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/beaglebone/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/beaglebone/include -L../../../../mach1spatial-libs/arm-chipsets/beaglebone/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-arm-cortexm0:
	g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include -L../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-armhf-neonvfpv4:
	g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include -L../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-rpi2:
	g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include -L../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-rpi3:
	g++ -o spatial-transcode-verbose-audioplayer src/main.cpp ../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include/Mach1Transcode.cpp -I../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include -L../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib -lstdc++ -pthread -lMach1TranscodeCAPI