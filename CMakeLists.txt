cmake_minimum_required(VERSION 3.15)

project(stream-cli-player LANGUAGES C CXX)

if (MSVC)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")
endif ()

add_executable(stream-server server/main.cpp)

configure_file(server/public/index.html ${PROJECT_BINARY_DIR}/Debug/public/index.html COPYONLY)
configure_file(server/public/styles.css ${PROJECT_BINARY_DIR}/Debug/public/styles.css COPYONLY)

configure_file(server/public/index.html ${PROJECT_BINARY_DIR}/Release/public/index.html COPYONLY)
configure_file(server/public/styles.css ${PROJECT_BINARY_DIR}/Release/public/styles.css COPYONLY)

target_include_directories(stream-server PRIVATE external/httplib/)



include(FetchContent)

FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG 0.12.0)
FetchContent_MakeAvailable(webview)

add_executable(player client/main.cpp)

target_include_directories(player PRIVATE external/httplib/)
target_include_directories(player PRIVATE external/subprocess.h/)
target_link_libraries(player PRIVATE webview::core)



add_executable(stream helper/main.cpp)
target_include_directories(stream PRIVATE external/subprocess.h/)


set_target_properties(stream-server PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(player PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(stream PROPERTIES LINKER_LANGUAGE CXX)
