target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpp>
  $<INSTALL_INTERFACE:include/mach1spatial>
)