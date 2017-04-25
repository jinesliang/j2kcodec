cmake_minimum_required(VERSION 3.5)
project(OpenJPEG-Wrapper)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
set(OPENJPEG_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/openjpeg/")
add_subdirectory(${OPENJPEG_ROOT_DIR})

# Include stuff from openjpeg
include_directories(
  ${OPENJPEG_BINARY_DIR}/src/lib/openjp2
  ${OPENJPEG_BINARY_DIR}/src/bin/common
  ${OPENJPEG_SOURCE_DIR}/src/lib/openjp2
  ${OPENJPEG_SOURCE_DIR}/src/bin/jp2
  ${OPENJPEG_SOURCE_DIR}/src/bin/common
  ${Z_INCLUDE_DIRNAME}
  ${PNG_INCLUDE_DIRNAME}
  ${TIFF_INCLUDE_DIRNAME}
  ${CMAKE_SOURCE_DIR}/include
)

add_executable(out ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/src/simplej2kcodec.cpp )
# Link to openjpeg
target_link_libraries(out openjp2)
