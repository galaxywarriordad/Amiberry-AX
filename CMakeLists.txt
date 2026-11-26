cmake_minimum_required(VERSION 3.25)

option(AMIBERRY_AX_USE_AX_SDK "Link ax::core from ax-sdk" ON)

if(AMIBERRY_AX_USE_AX_SDK AND NOT TARGET ax::core)
    include(FetchContent)
    set(AX_SDK_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ax-sdk")
    if(EXISTS "${AX_SDK_LOCAL_DIR}/CMakeLists.txt")
        add_subdirectory("${AX_SDK_LOCAL_DIR}" "${CMAKE_BINARY_DIR}/_deps/ax_sdk-build" EXCLUDE_FROM_ALL)
    else()
        FetchContent_Declare(
            ax_sdk
            GIT_REPOSITORY https://github.com/galaxywarriordad/ax-sdk.git
            GIT_TAG v1_36_2
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(ax_sdk)
    endif()
endif()

set(_amiberry_ax_persist_src "${CMAKE_CURRENT_SOURCE_DIR}/src/ax_persist_callers.cpp")
if(EXISTS "${_amiberry_ax_persist_src}")
    add_library(amiberry_ax_persist STATIC
        "${_amiberry_ax_persist_src}"
    )
    target_compile_features(amiberry_ax_persist PRIVATE cxx_std_20)
    target_include_directories(amiberry_ax_persist PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
    if(AMIBERRY_AX_USE_AX_SDK AND TARGET ax::core)
        target_link_libraries(amiberry_ax_persist PUBLIC ax::core)
    endif()

    foreach(_amiberry_target IN ITEMS amiberry amiberry-ax)
        if(TARGET "${_amiberry_target}")
            target_link_libraries("${_amiberry_target}" PRIVATE amiberry_ax_persist)
        endif()
    endforeach()
endif()
