cmake_minimum_required(VERSION 3.20)

project(untitled VERSION 1.0.0 LANGUAGES CXX)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.hxx.in"
    "${PROJECT_SOURCE_DIR}/include/config.hxx"
)

add_subdirectory(external/glm EXCLUDE_FROM_ALL)
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
add_subdirectory(external/glfw EXCLUDE_FROM_ALL)
add_subdirectory(external/nlohmann_json EXCLUDE_FROM_ALL)

include_directories(external)
include_directories(external/glm)
include_directories(external/fmt)
include_directories(external/glfw)
include_directories(external/nlohmann_json)

#set(glm_DIR "${PROJECT_SOURCE_DIR}/cmake")

set(EXECUTABLE_TARGET_NAME executable)
add_executable(${EXECUTABLE_TARGET_NAME})

target_sources(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        main.cpp
)

message("${CMAKE_PREFIX_PATH}")

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Vulkan REQUIRED)

target_include_directories(${EXECUTABLE_TARGET_NAME} SYSTEM PRIVATE include)
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE src)

target_compile_features(${EXECUTABLE_TARGET_NAME}
    PUBLIC
        cxx_std_20
)

set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}

    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF

    POSITION_INDEPENDENT_CODE ON

    DEBUG_POSTFIX .d
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        Vulkan::Vulkan

#        fmt::fmt-header-only
        fmt::fmt
        glfw
        glm#::glm
        nlohmann_json::nlohmann_json

#        range-v3 range-v3-meta range-v3::meta range-v3-concepts
)
