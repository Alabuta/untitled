cmake_minimum_required(VERSION 3.20)

project(untitled VERSION 1.0.0 LANGUAGES CXX)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.hxx.in"
    "${PROJECT_SOURCE_DIR}/include/config.hxx"
)

set(SUBMODULES
    glm
    fmt
    glfw
    volk
    range-v3
    nlohmann_json
)

foreach(SUBMODULE ${SUBMODULES})
    message(STATUS "Including submodule ${SUBMODULE}")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/${SUBMODULE} EXCLUDE_FROM_ALL)
endforeach()

find_package(Vulkan REQUIRED)
find_package(Boost 1.68 REQUIRED)

set(EXECUTABLE_TARGET_NAME executable)
add_executable(${EXECUTABLE_TARGET_NAME})

target_sources(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        main.cxx
)

include_directories(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        src

    SYSTEM INTERFACE
        ${Boost_INCLUDE_DIR}

        ${CMAKE_CURRENT_LIST_DIR}/external
        ${CMAKE_CURRENT_LIST_DIR}/external/glm
        ${CMAKE_CURRENT_LIST_DIR}/external/fmt
        ${CMAKE_CURRENT_LIST_DIR}/external/glfw
        ${CMAKE_CURRENT_LIST_DIR}/external/volk
        ${CMAKE_CURRENT_LIST_DIR}/external/range-v3
        ${CMAKE_CURRENT_LIST_DIR}/external/nlohmann_json
        include
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    target_compile_definitions(${EXECUTABLE_TARGET_NAME}
        PRIVATE
            _GLIBCXX_USE_CXX11_ABI=1
    )
endif ()

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

        ${Boost_LIBRARIES}

        glm::glm
        glfw
        volk
        fmt::fmt
        range-v3 range-v3-meta range-v3::meta range-v3-concepts
        nlohmann_json::nlohmann_json
)
