cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION FILC_VERSION)

project(
        filc
        VERSION ${FILC_VERSION}
        LANGUAGES CXX
)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Library

add_library(additional_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # No optimization + debug info + coverage flags
    target_compile_options(additional_config INTERFACE -O0 -g --coverage)
    target_link_options(additional_config INTERFACE --coverage)
else ()
    # Full optimization
    target_compile_options(additional_config INTERFACE -O3)
endif ()

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG 3bf268481da8208d171d8908e6491459de3651d7 #v3.2.0
)
FetchContent_MakeAvailable(cxxopts)
message(DEBUG cxxopts="${cxxopts_SOURCE_DIR}/include")

add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_TAG 4.13.2)
set(ANTLR4_ZIP_REPOSITORY https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR4_TAG}.zip)
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/bin/antlr-${ANTLR4_TAG}-complete.jar)

include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})
find_package(ANTLR REQUIRED)

antlr_target(Lexer ${PROJECT_SOURCE_DIR}/src/grammar/FilLexer.g4 LEXER
        PACKAGE filc)
antlr_target(Parser ${PROJECT_SOURCE_DIR}/src/grammar/FilParser.g4 PARSER
        PACKAGE filc
        DEPENDS_ANTLR Lexer
        COMPILE_FLAGS -lib ${ANTLR_Lexer_OUTPUT_DIR})

file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
)
message(DEBUG SRC_FILES=${SRC_FILES})

add_library(filc_lib ${SRC_FILES} ${ANTLR_Lexer_CXX_OUTPUTS} ${ANTLR_Parser_CXX_OUTPUTS})
target_link_libraries(filc_lib PRIVATE additional_config cxxopts::cxxopts antlr4_static)

target_include_directories(filc_lib PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${cxxopts_SOURCE_DIR}/include"
        ${ANTLR4_INCLUDE_DIRS}
        ${ANTLR_Lexer_OUTPUT_DIR}
        ${ANTLR_Parser_OUTPUT_DIR})

target_compile_definitions(filc_lib
        PUBLIC FILC_VERSION="${FILC_VERSION}"
)

target_compile_options(filc_lib PUBLIC -Wall)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Executable

add_executable(filc main.cpp)
target_link_libraries(filc PRIVATE filc_lib)

target_compile_options(filc PUBLIC -Wall -O3)

add_custom_target(filc_path ALL "echo" $<TARGET_FILE:filc> ">" "filc.path")
add_dependencies(filc_path filc)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Tests

option(BUILD_TESTING "Build the testing tree." OFF)
if (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    message(DEBUG "Build test tree")
    enable_testing()
    add_subdirectory(tests)
endif ()

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Packaging

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(DEBUG "Build install tree")
    install(TARGETS filc)
    include(packaging)
    filc_package()
endif ()
