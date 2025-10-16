# Mach1Encode GUI Test Tool
# This tool provides a graphical interface for testing Mach1Encode parameters
# and visualizing the output gain coefficients and spatial configurations

cmake_minimum_required(VERSION 3.13.0)
project(Mach1EncodeGUITest)

# Include the main SDK build system
list(APPEND CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(common-defaults)

# Include libMach1Spatial to get access to M1Encode target
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../libmach1spatial ${CMAKE_CURRENT_BINARY_DIR}/libmach1spatial)

# Find required packages
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

message(STATUS "All required packages found:")
message(STATUS "  OpenGL: ${OpenGL_LIBRARIES}")
message(STATUS "  GLFW: glfw")
message(STATUS "  GLEW: GLEW::GLEW")

# Find ImGui (we'll include it as a submodule or download)
find_path(IMGUI_INCLUDE_DIR imgui.h
    HINTS ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui
    PATH_SUFFIXES include
)

if(NOT IMGUI_INCLUDE_DIR)
    message(STATUS "ImGui not found, downloading from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.90.4
    )
    FetchContent_MakeAvailable(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR})
    set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    message(STATUS "ImGui downloaded and configured")
else()
    message(STATUS "ImGui found at: ${IMGUI_INCLUDE_DIR}")
    # Assume ImGui is already built or we need to find the sources
    set(IMGUI_SOURCES "")
endif()

# Create the GUI test executable
add_executable(Mach1EncodeGUITest
    src/main.cpp
    src/Mach1EncodeGUI.cpp
    src/VisualizationWidget.cpp
    src/ParameterControls.cpp
    ${IMGUI_SOURCES}
)

# Link libraries
target_link_libraries(Mach1EncodeGUITest PRIVATE 
    M1Encode
    OpenGL::GL
    glfw
    GLEW::GLEW
)

# Include directories
target_include_directories(Mach1EncodeGUITest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmach1spatial/api_encode/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmach1spatial/api_common/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmach1spatial/deps/glm
    ${IMGUI_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}/backends
)

# Compiler definitions
target_compile_definitions(Mach1EncodeGUITest PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLEW
)

# Set C++ standard
set_target_properties(Mach1EncodeGUITest PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(Mach1EncodeGUITest PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(Mach1EncodeGUITest PRIVATE "-framework OpenGL")
elseif(UNIX)
    target_link_libraries(Mach1EncodeGUITest PRIVATE GL)
endif()

# Copy shaders and resources if needed
# add_custom_command(TARGET Mach1EncodeGUITest POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources
#     $<TARGET_FILE_DIR:Mach1EncodeGUITest>/resources
# )
