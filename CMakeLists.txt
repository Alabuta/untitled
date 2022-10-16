cmake_minimum_required(VERSION 3.25)

project(untitled VERSION 1.0.0 LANGUAGES CXX)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.hxx.in"
    "${PROJECT_SOURCE_DIR}/include/config.hxx"
)

set(EXECUTABLE_TARGET_NAME executable)
add_executable(${EXECUTABLE_TARGET_NAME})

set(CMAKE_LINK_LIBRARY_USING_load_archive TRUE)
set(CMAKE_C_LINK_LIBRARY_USING_load_archive_SUPPORTED TRUE)
set(CMAKE_CXX_LINK_LIBRARY_USING_load_archive_SUPPORTED TRUE)

# Get compiler info
set(CXX_FLAGS_STYLE_GNU OFF)
set(CXX_FLAGS_STYLE_MSVC OFF)
set(CXX_FLAGS_STYLE_CLANGCL OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CXX_FLAGS_STYLE_GNU ON)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
        set(CXX_FLAGS_STYLE_CLANGCL ON)
    else()
        set(CXX_FLAGS_STYLE_GNU ON)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CXX_FLAGS_STYLE_MSVC ON)
else()
    message(FATAL_ERROR "Unsupported compiler!")
endif()

set(GLFW_LIBRARY_TYPE "SHARED" CACHE STRING "" FORCE)

#[[set(USE_ASAN OFF)
if(${USE_ASAN})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -g3")

    set(EXTRA_COMPILER_OPTIONS ${EXTRA_COMPILER_OPTIONS}
        -fsanitize=address -g3
    )

    set(EXTRA_LIBS ${EXTRA_LIBS}
        clang_rt.asan_dynamic-x86_64
        clang_rt.asan_dynamic_runtime_thunk-x86_64
    )

    set(EXTRA_LINK_OPTIONS ${EXTRA_LINK_OPTIONS}
        /wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib
    )

    set(EXTRA_LINK_DIRS ${EXTRA_LINK_DIRS}
        "C:/Program Files/LLVM/lib/clang/15.0.2/lib/windows"
    )
endif()]]

set(SUBMODULES
    glm
    volk
    glfw
    fmt
    nlohmann_json
    # range-v3
)

foreach(SUBMODULE ${SUBMODULES})
    message(STATUS "Including submodule ${SUBMODULE}")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/${SUBMODULE} EXCLUDE_FROM_ALL SYSTEM)
endforeach()

find_package(Vulkan REQUIRED)
find_package(Boost 1.68 REQUIRED)

target_sources(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        src/main.cxx
)

target_include_directories(${EXECUTABLE_TARGET_NAME}
    SYSTEM PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/external
        include

    PRIVATE
        ${Boost_INCLUDE_DIR}
        src
)

target_compile_options(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        ${EXTRA_COMPILER_OPTIONS}

        "$<$<OR:$<BOOL:${CXX_FLAGS_STYLE_GNU}>,$<BOOL:${CXX_FLAGS_STYLE_CLANGCL}>>:"
            -Wpedantic
            -Wall
            -Wextra
            -Werror

            -Wold-style-cast
            -Wnon-virtual-dtor
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast
            -Wmisleading-indentation

            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic

            -Wno-pre-c++17-compat
        ">"

        "$<$<BOOL:${CXX_FLAGS_STYLE_GNU}>:"
            -fconcepts

            -fasynchronous-unwind-tables                # Increased reliability of backtraces
            -fexceptions                                # Enable table-based thread cancellation
            # -fPIE
            # -fpie

            -pipe
        ">"

        "$<$<BOOL:${CXX_FLAGS_STYLE_CLANGCL}>:"
            /EHa
            # -Xclang -fexceptions -Xclang -fcxx-exceptions
            SHELL:-Xclang -fexceptions

            -Wno-unknown-pragmas
            -Wno-unknown-warning-option

            -Wno-shadow-field-in-constructor
        ">"

        "$<$<BOOL:${CXX_FLAGS_STYLE_MSVC}>:"
            "/std:c++latest"

            # /W4
            # /WX
            /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
            /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
            /w14263 # 'function': member function does not override any base class virtual member function
            /w14265 # 'classname': class has virtual functions, but destructor is not virtual
            /w14287 # 'operator': unsigned/negative constant mismatch
            /we4289 # 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
            /w14296 # 'operator': expression is always 'boolean_value'
            /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
            /w14545 # expression before comma evaluates to a function which is missing an argument list
            /w14546 # function call before comma missing argument list
            /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
            /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
            /w14555 # expression has no effect; expected expression with side-effect
            /w14619 # pragma warning: there is no warning number 'number'
            /w14640 # Enable warning on thread un-safe static member initialization
            /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
            /w14905 # wide string literal cast to 'LPSTR'
            /w14906 # string literal cast to 'LPWSTR'
            /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        ">"
)

set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}

    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF

    POSITION_INDEPENDENT_CODE ON

    DEBUG_POSTFIX .d
)

target_link_directories(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        ${EXTRA_LINK_DIRS}
        # "C:/Program Files/LLVM/lib/clang/15.0.2/lib/windows"
)

if(${USE_ASAN})
    if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
        set(CMAKE_C_LINK_LIBRARY_USING_load_archive "-force_load clang_rt.asan_dynamic_runtime_thunk-x86_64.lib")
    elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_C_LINK_LIBRARY_USING_load_archive
            "LINKER:--push-state,--whole-archive"
            "clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"
            "LINKER:--pop-state"
        )
    elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_C_LINK_LIBRARY_USING_load_archive "/WHOLEARCHIVE:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib")
    else()
        # feature not yet supported for the other environments
        set(CMAKE_C_LINK_LIBRARY_USING_load_archive_SUPPORTED FALSE)
    endif()

    # The -force_load Apple linker option requires a file name
    set(asan_dynamic_runtime_thunk_lib
        "$<IF:$<LINK_LANG_AND_ID:C,AppleClang>,clang_rt.asan_dynamic_runtime_thunk-x86_64.a,clang_rt.asan_dynamic_runtime_thunk-x86_64>"
    )
endif()

target_link_libraries(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        ${EXTRA_LIBS}
        #[[clang_rt.asan_dynamic-x86_64
        clang_rt.asan_dynamic_runtime_thunk-x86_64.lib]]
        # "$<LINK_LIBRARY:load_archive,${asan_dynamic_runtime_thunk_lib}>"

        Vulkan::Vulkan
        ${Boost_LIBRARIES}
        glm::glm
        volk_headers
        glfw
        fmt::fmt-header-only
        nlohmann_json::nlohmann_json
        # range-v3 range-v3-meta range-v3::meta range-v3-concepts
)

target_link_options(${EXECUTABLE_TARGET_NAME}
    PRIVATE
        ${EXTRA_LINK_OPTIONS}

        "$<$<IN_LIST:${CMAKE_SYSTEM_NAME}," Linux";"Darwin">:"
            # LINKER:SHELL:-z defs
            LINKER:-z,defs;                         # Detect and reject underlinking
            LINKER:-z,now;                          # Disable lazy binding
            LINKER:-z,relro                         # Read-only segments after relocation
        ">"

        "$<$<OR:$<BOOL:${CXX_FLAGS_STYLE_GNU}>,$<BOOL:${CXX_FLAGS_STYLE_CLANGCL}>>:"
            LINKER:-pie;
            LINKER:-no-undefined;                   # Report unresolved symbol references from regular object files
            LINKER:-no-allow-shlib-undefined;       # Disallows undefined symbols in shared libraries
            LINKER:-unresolved-symbols=report-all
        ">"
)
