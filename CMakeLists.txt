cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

# Nix dependencies
file(GLOB NIX_STORE_LIB LIST_DIRECTORIES true "/nix/store/*/lib/cmake/*")
message(DEBUG NIX_STORE_LIB=${NIX_STORE_LIB})
list(APPEND CMAKE_PREFIX_PATH ${NIX_STORE_LIB})
file(GLOB NIX_STORE_BIN LIST_DIRECTORIES true "/nix/store/*/bin/")
message(DEBUG NIX_STORE_BIN=${NIX_STORE_BIN})
list(APPEND CMAKE_PROGRAM_PATH ${NIX_STORE_BIN})

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

include(commands)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Library

## Coverage config
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

## Cxxopts
find_package(cxxopts 3.2.0 REQUIRED CONFIG)
message(STATUS "Found cxxopts ${cxxopts_PACKAGE_VERSION}")
message(STATUS "Located in: ${cxxopts_DIR}")
get_target_property(cxxopts_INCLUDE_DIR cxxopts::cxxopts INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Included from: ${cxxopts_INCLUDE_DIR}")

## Antlr4
find_package(antlr4-runtime 4.13.0 REQUIRED CONFIG)
message(STATUS "Found antlr ${ANTLR_VERSION}")
message(STATUS "Located in: ${antlr4-runtime_DIR}")
message(STATUS "Included from: ${ANTLR4_INCLUDE_DIR}")

find_program(ANTLR_EXECUTABLE antlr REQUIRED)
message(STATUS "Found antlr: ${ANTLR_EXECUTABLE}")

antlr_target(Lexer ${PROJECT_SOURCE_DIR}/src/grammar/FilLexer.g4 LEXER
        PACKAGE filc)
antlr_target(Parser ${PROJECT_SOURCE_DIR}/src/grammar/FilParser.g4 PARSER
        PACKAGE filc
        DEPENDS_ANTLR Lexer
        COMPILE_FLAGS -lib ${ANTLR_Lexer_OUTPUT_DIR})

## LLVM
find_package(LLVM 18.1.8 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Located in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

foreach(target ${LLVM_TARGETS_TO_BUILD})
    list(APPEND llvm_targets "LLVM${target}CodeGen")
endforeach()

## filc lib
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS filc_lib
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
)
message(DEBUG SRC_FILES=${SRC_FILES})

add_library(filc_lib ${SRC_FILES} ${ANTLR_Lexer_CXX_OUTPUTS} ${ANTLR_Parser_CXX_OUTPUTS})
target_link_libraries(filc_lib PRIVATE additional_config cxxopts::cxxopts antlr4_static ${llvm_libs} ${llvm_targets})

target_include_directories(filc_lib PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        ${cxxopts_INCLUDE_DIR}
        ${ANTLR4_INCLUDE_DIR}
        ${ANTLR_Lexer_OUTPUT_DIR}
        ${ANTLR_Parser_OUTPUT_DIR}
        ${LLVM_INCLUDE_DIRS})

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
