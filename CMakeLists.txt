cmake_minimum_required(VERSION 3.20)
project(HiBitAudio VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED
    src/dllmain.cpp
    src/resource_manager.cpp
    src/audio_processor.cpp
    src/vorbis_proxy.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    include/pch.h
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX-
        /O2
        /GS-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "libvorbisfile_64"
    SUFFIX ".dll"
)
