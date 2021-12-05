cmake_minimum_required(VERSION 3.20)

include(ExternalProject)

project(untitled VERSION 1.0.0 LANGUAGES CXX)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.hxx.in"
    "${PROJECT_SOURCE_DIR}/include/config.hxx"
)

set(Boost_Bootstrap_Command)
if(UNIX)
    set(Boost_Bootstrap_Command ./bootstrap.sh)
    set(Boost_b2_Command ./b2)
elif(WIN32)
    set(Boost_Bootstrap_Command "bootstrap.bat vc141")
    set(Boost_b2_Command b2.exe)
endif()

ExternalProject_Add(boost
    PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/external/boost
    URL                 https://sourceforge.net/projects/boost/files/boost/1.77.0/boost_1_77_0.zip
#    GIT_REPOSITORY      https://github.com/boostorg/boost.git
#    GIT_TAG             "boost-1.77.0"
    UPDATE_COMMAND      ""
    PATCH_COMMAND       ""
    CONFIGURE_COMMAND   ${Boost_Bootstrap_Command}
#    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE     ON
#    BUILD_COMMAND       bjam --with-program_options address-model=64 toolset=msvc-11.0 link=static stage
    BUILD_COMMAND       ${Boost_b2_Command} #install
                            --without-python
                            --without-mpi
                            --disable-icu
                            --prefix=${CMAKE_CURRENT_BINARY_DIR}/INSTALL
                            --threading=single,multi
                            --link=static#shared
                            --variant=debug
                            -j16
    INSTALL_COMMAND     ""
)

set(Boost_LIBRARY_DIR ${CMAKE_BINARY_DIR}/INSTALL/lib/)
set(Boost_INCLUDE_DIR ${CMAKE_BINARY_DIR}/INSTALL/include/boost-1_77/)

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
#find_package(Boost 1.68 REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
#    target_link_libraries(${EXECUTABLE_TARGET_NAME} ${Boost_LIBRARIES})
endif ()

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
