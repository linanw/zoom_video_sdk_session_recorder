cmake_minimum_required(VERSION 3.14)

project(VideoSDKSessionRecorder CXX)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# add_definitions(-std=c++11)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0 libavcodec libavformat libavutil libswscale)
find_package(PahoMqttCpp REQUIRED)
find_package(OpenCV REQUIRED)


# Prepare the required include directories for compiling
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/zoom_video_sdk)
include_directories(${OpenCV_INCLUDE_DIRS})

# Prepare the required libraries for linking
# Extract the required libraries from archives to the lib directory
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk")
    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk COMMAND tar -xf ${CMAKE_SOURCE_DIR}/lib.archive/zoom_video_sdk.tar.gz -C lib/zoom_video_sdk WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/hikvision_sdk")
    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/lib/hikvision_sdk COMMAND tar -xf ${CMAKE_SOURCE_DIR}/lib.archive/hikvision_sdk.tar.gz -C lib/hikvision_sdk WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

link_directories(${CMAKE_SOURCE_DIR}/lib/hikvision_sdk)

# Add the executable
add_executable(video_sdk_session_recorder
${CMAKE_SOURCE_DIR}/src/zoom_video_sdk_delegate.cpp
    # ${CMAKE_SOURCE_DIR}/src/raw_data_ffmpeg_encoder.cpp
    ${CMAKE_SOURCE_DIR}/src/hikvision_camera.cpp
    ${CMAKE_SOURCE_DIR}/src/hikvision_video_source.cpp
    ${CMAKE_SOURCE_DIR}/src/yuv_writer.cpp
    ${CMAKE_SOURCE_DIR}/src/video_sdk_session_recorder.cpp)

target_link_libraries(video_sdk_session_recorder PkgConfig::deps)
target_link_libraries(video_sdk_session_recorder ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/libvideosdk.so.1)
target_link_libraries(video_sdk_session_recorder hcnetsdk)
target_link_libraries(video_sdk_session_recorder paho-mqttpp3)
target_link_libraries(video_sdk_session_recorder ${OpenCV_LIBS})

# Prepare for running the executable
configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_SOURCE_DIR}/bin/config.json COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/lib/zoom_video_sdk/ DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/lib/hikvision_sdk/ DESTINATION ${CMAKE_SOURCE_DIR}/bin)
execute_process(COMMAND mkdir -p .zoom/logs WORKING_DIRECTORY $ENV{HOME})

# Set RPATH to look in the same directory as the executable ($ORIGIN)
set_target_properties(video_sdk_session_recorder PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN"
)
