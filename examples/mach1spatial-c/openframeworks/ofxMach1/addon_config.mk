# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file
# system
#
# The PG can be used to detect errors in this file, just create a new project with this addon
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxMach1
	ADDON_DESCRIPTION = Addon for Mach1 Spatial VVBP(SPS)
	ADDON_AUTHOR = Mach1
	ADDON_TAGS = "spatial audio" "spatial functions"
	ADDON_URL = http://mach1.tech

common:
	# dependencies with other addons, a list of them separated by spaces
	# or use += in several lines
	# ADDON_DEPENDENCIES =

	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	# ADDON_INCLUDES =

	# any special flag that should be passed to the compiler when using this
	# addon
	# ADDON_CFLAGS =

	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	# ADDON_LDFLAGS =

	# linux only, any library that should be included in the project using
	# pkg-config
	# ADDON_PKG_CONFIG_LIBRARIES =

	# osx/iOS only, any framework that should be included in the project
	# ADDON_FRAMEWORKS =

	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	# ADDON_SOURCES =

	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	# ADDON_DATA =

	# when parsing the file system looking for libraries exclude this for all or
	# a specific platform
	# ADDON_LIBS_EXCLUDE =

	ADDON_DEFINES += M1_STATIC

linux:
	ADDON_LIBS += libs/libmach1/lib/linux/libMach1DecodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/linux/libMach1EncodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/linux/libMach1DecodePositionalCAPI.a
	ADDON_LIBS += libs/libmach1/lib/linux/libMach1TranscodeCAPI.a
linux64:
linuxarmv6l:
linuxarmv7l:
android/armeabi-v7a:
	ADDON_LIBS += libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a
android/x86:
	ADDON_LIBS += libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a
	ADDON_LIBS += libs/libmach1/lib/android/libMach1TranscodeCAPI.a
osx:
	ADDON_LIBS += libs/libmach1/lib/osx/libMach1DecodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/osx/libMach1EncodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a
	ADDON_LIBS += libs/libmach1/lib/osx/libMach1TranscodeCAPI.a
ios:
	ADDON_LIBS += libs/libmach1/lib/ios/libMach1DecodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/ios/libMach1EncodeCAPI.a
	ADDON_LIBS += libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a
	ADDON_LIBS += libs/libmach1/lib/ios/libMach1TranscodeCAPI.a
tvos:
msys2:
vs:
	ADDON_LIBS += libs/libmach1/lib/vs/x64/Mach1DecodeCAPI.lib
	ADDON_LIBS += libs/libmach1/lib/vs/x64/Mach1EncodeCAPI.lib
	ADDON_LIBS += libs/libmach1/lib/vs/x64/Mach1DecodePositionalCAPI.lib
	ADDON_LIBS += libs/libmach1/lib/vs/x64/Mach1TranscodeCAPI.lib
