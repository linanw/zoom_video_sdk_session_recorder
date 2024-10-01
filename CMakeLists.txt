cmake_minimum_required(VERSION 3.14)

project(VideoSDKSendRawData CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0)
find_package(OpenCV REQUIRED)


# Prepare the required include directories for compiling
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/zoom_video_sdk)
include_directories(${OpenCV_INCLUDE_DIRS})

# Prepare the required libraries for linking
# Extract the required libraries from archives to the lib directory
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/libvideosdk.so.1" OR NOT EXISTS "${CMAKE_SOURCE_DIR}/include/zoom_video_sdk")
    execute_process(
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/include/zoom_video_sdk)
    execute_process(
        COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk
        COMMAND tar -xf ${CMAKE_SOURCE_DIR}/lib.archive/zoom-video-sdk-linux_x86_64-1.13.3.tar.xz -C lib/zoom_video_sdk WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    execute_process(
        COMMAND mv ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/libvideosdk.so ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/libvideosdk.so.1
        COMMAND mv ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/h ${CMAKE_SOURCE_DIR}/include/zoom_video_sdk)
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/config.json")
    execute_process(
        COMMAND cp ${CMAKE_SOURCE_DIR}/config.json.example ${CMAKE_SOURCE_DIR}/config.json)
endif()

# Add the executable
add_executable(video_sdk_send_raw_data
${CMAKE_SOURCE_DIR}/src/zoom_video_sdk_delegate.cpp
    ${CMAKE_SOURCE_DIR}/src/opencv_video_source.cpp
    ${CMAKE_SOURCE_DIR}/src/video_sdk_send_raw_data.cpp)

target_link_libraries(video_sdk_send_raw_data PkgConfig::deps)
target_link_libraries(video_sdk_send_raw_data ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/libvideosdk.so.1)
target_link_libraries(video_sdk_send_raw_data ${OpenCV_LIBS})

# Prepare for running the executable
configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_SOURCE_DIR}/bin/config.json COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/ DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/Big_Buck_Bunny_1080_10s_1MB.mp4 DESTINATION ${CMAKE_SOURCE_DIR}/bin)
execute_process(COMMAND mkdir -p .zoom/logs WORKING_DIRECTORY $ENV{HOME})

# Set RPATH to look in the same directory as the executable ($ORIGIN)
set_target_properties(video_sdk_send_raw_data PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN"
)
