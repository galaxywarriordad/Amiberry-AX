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

add_library(amiberry_ax_persist STATIC
    src/ax_persist_callers.cpp
)
target_compile_features(amiberry_ax_persist PRIVATE cxx_std_20)
target_include_directories(amiberry_ax_persist PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
if(TARGET ax::core)
    target_link_libraries(amiberry_ax_persist PUBLIC ax::core)
endif()

# Link amiberry_ax_persist into the main Amiberry executable target:
# target_link_libraries(amiberry PRIVATE amiberry_ax_persist)
