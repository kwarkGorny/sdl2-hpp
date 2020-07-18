cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Building in-tree is not allowed.")
    return()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(EXTERNAL_LOCATION ${CMAKE_SOURCE_DIR}/external/win)

project(
    "sdl2-hpp"
    DESCRIPTION "simple C++ binding for sdl2"
    LANGUAGES CXX C
)

include(Settings)
include(Utils)
include(Warnings)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

list(APPEND libs PUBLIC
    project_warnings
)

add_source_folder(src/sdl2)
add_source_folder(src/sdl2/image)
add_source_folder(src/sdl2/mixer)
add_source_folder(src/sdl2/ttf)

list(APPEND includes PUBLIC
    src/
)

set(SDL_LOCATION ${EXTERNAL_LOCATION})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable (${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} ${includes})

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL2::Main
    SDL2::Image
    SDL2::TTF
    SDL2::Mixer
    ${libs}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying required dlls.."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLLS}
    ${SDL2_IMAGE_DLLS}
    ${SDL2_TTF_DLLS}
    ${SDL2_MIXER_DLLS}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")