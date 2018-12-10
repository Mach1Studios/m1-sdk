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
	ADDON_NAME = ofxAudioDecoder
	ADDON_DESCRIPTION = =
	ADDON_AUTHOR = =
	ADDON_TAGS = =
	ADDON_URL = =

vs:
	ADDON_LDFLAGS += -lMfreadwrite.lib -lMFPlat.lib -lmfuuid.lib
	ADDON_SOURCES_EXCLUDE = libs/audiodecoder/src/audiodecodercoreaudio.cpp
	ADDON_SOURCES_EXCLUDE += libs/audiodecoder/include/audiodecodercoreaudio.h
	ADDON_SOURCES_EXCLUDE += libs/audiodecoder/include/apple/%

	ADDON_LDFLAGS += -lMfreadwrite.lib -lMFPlat.lib -lmfuuid.lib

osx:
    ADDON_SOURCES += libs/libaudiodecoder/include/apple/%

	ADDON_SOURCES_EXCLUDE += libs/audiodecoder/src/audiodecodermediafoundation.cpp
	ADDON_SOURCES_EXCLUDE += libs/audiodecoder/include/audiodecodermediafoundation.h
