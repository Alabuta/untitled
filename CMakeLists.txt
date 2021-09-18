cmake_minimum_required(VERSION 3.20)

project(untitled VERSION 1.0.0 LANGUAGES CXX)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.hxx.in"
    "${PROJECT_SOURCE_DIR}/include/config.hxx"
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/glm EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/fmt EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/volk EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/range-v3 EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/nlohmann_json EXCLUDE_FROM_ALL)

set(EXECUTABLE_TARGET_NAME executable)
add_executable(${EXECUTABLE_TARGET_NAME})

target_sources(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        main.cpp
)

message("${CMAKE_PREFIX_PATH}")

find_package(Vulkan REQUIRED)

include_directories(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        src

    SYSTEM INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/external
        ${CMAKE_CURRENT_LIST_DIR}/external/glm
        ${CMAKE_CURRENT_LIST_DIR}/external/fmt
        ${CMAKE_CURRENT_LIST_DIR}/external/glfw
        ${CMAKE_CURRENT_LIST_DIR}/external/volk
        ${CMAKE_CURRENT_LIST_DIR}/external/range-v3
        ${CMAKE_CURRENT_LIST_DIR}/external/nlohmann_json
        include
)

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

        glm#::glm
        glfw
        volk
        fmt::fmt
        range-v3 range-v3-meta range-v3::meta range-v3-concepts
        nlohmann_json::nlohmann_json
)
