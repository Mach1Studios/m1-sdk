# Mach1 Spatial SDK
# Compile and test Mach1Transcode API

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
		g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/linux/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
	ifeq ($(UNAME_S),Darwin)
		g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/xcode/lib -lstdc++ -lpthread -lMach1TranscodeCAPI
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/linux/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/linux-arm/lib -lstdc++ -pthread -lMach1TranscodeCAPI
	endif
endif

build-beaglebone:
	g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/arm/beaglebone/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-arm-cortexm0:
	g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/arm/gcc-arm-cortexm0-cxx14/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-armhf-neonvfpv4:
	g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/arm/gcc-armhf-neon-vfpv4/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-rpi2:
	g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/arm/raspberrypi2-cxx11/lib -lstdc++ -pthread -lMach1TranscodeCAPI

build-rpi3:
	g++ -o spatial-transcode-example-test spatial-transcode-example/main.cpp ../../../../include/cpp/Mach1Transcode.cpp -I../../../../include/cpp -L../../../../binaries/arm/raspberrypi3-cxx11/lib -lstdc++ -pthread -lMach1TranscodeCAPI
