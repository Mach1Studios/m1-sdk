# Mach1 Spatial SDK
# Compile and test Mach1Decode Minified API

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
		g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/linux/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/linux/include -L../../../../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1DecodeCAPI
	endif
	ifeq ($(UNAME_S),Darwin)
		g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/xcode/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/xcode/include -L../../../../mach1spatial-libs/xcode/lib -lstdc++ -lpthread -lMach1DecodeCAPI
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/linux/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/linux/include -L../../../../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1DecodeCAPI
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/linux-arm/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/linux-arm/include -L../../../../mach1spatial-libs/arm-chipsets/linux-arm/lib -lstdc++ -pthread -lMach1DecodeCAPI
	endif
endif

build-beaglebone:
	g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/beaglebone/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/beaglebone/include -L../../../../mach1spatial-libs/arm-chipsets/beaglebone/lib -lstdc++ -pthread -lMach1DecodeCAPI

build-arm-cortexm0:
	g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include -L../../../../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib -lstdc++ -pthread -lMach1DecodeCAPI

build-armhf-neonvfpv4:
	g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include -L../../../../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib -lstdc++ -pthread -lMach1DecodeCAPI

build-rpi2:
	g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include -L../../../../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib -lstdc++ -pthread -lMach1DecodeCAPI

build-rpi3:
	g++ -o spatial-decode-minified-example-test spatial-decodeminified-example/main.cpp ../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include/Mach1Decode.cpp -I../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include -L../../../../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib -lstdc++ -pthread -lMach1DecodeCAPI
