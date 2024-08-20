message(STATUS "Configuring Mach1 Spatial SDK examples:")

if(WIN32 OR MSVC OR MINGW)
    add_compile_definitions(M1_STATIC)
    message(STATUS "Adding windows OS flags")
endif()

option(M1ENCODE_INLINE_DECODE    "Support M1Encode with built in Decode functions" OFF)
add_subdirectory(spatial-encode-example)

add_subdirectory(spatial-decode-example)
add_subdirectory(spatial-decodepositional-example)
add_subdirectory(spatial-transcode-example)
add_subdirectory(spatial-transcode-objectaudio-example)
add_subdirectory(spatial-transcode-render-example)
add_subdirectory(spatial-transcode-audioplayer)
add_subdirectory(spatial-transcode-verbose-audioplayer)
