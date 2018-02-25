cmake_minimum_required(VERSION 3.5)
project(OpenJPEG-Wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
set(GROK_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/grok/")
add_subdirectory(${GROK_ROOT_DIR})

# Include stuff from grok
include_directories(
  ${GROK_ROOT_DIR}/src/lib/openjp2
  ${GROK_ROOT_DIR}/src/bin/common
  ${Z_INCLUDE_DIRNAME}
  ${PNG_INCLUDE_DIRNAME}
  ${TIFF_INCLUDE_DIRNAME}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/ext/xml2json/include
  ${CMAKE_SOURCE_DIR}/build/ext/grok/src/lib/openjp2
)

add_executable(extract_json_from_jp2 ${PROJECT_SOURCE_DIR}/test/extract_json_from_jp2.cpp ${PROJECT_SOURCE_DIR}/src/j2kcodec.cpp )
add_executable(extract_json_from_jp2_rec ${PROJECT_SOURCE_DIR}/test/extract_json_from_jp2_rec.cpp ${PROJECT_SOURCE_DIR}/src/j2kcodec.cpp )

# Link to openjpeg
target_link_libraries(extract_json_from_jp2 openjp2)
target_link_libraries(extract_json_from_jp2_rec openjp2)
