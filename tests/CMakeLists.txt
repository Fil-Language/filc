FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 #v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# Unit tests

file(GLOB_RECURSE UNIT_TEST_FILES
        "${PROJECT_SOURCE_DIR}/tests/unit/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/unit/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/unit/*.h"
        "${PROJECT_SOURCE_DIR}/tests/unit/**/*.h"
)
message(DEBUG UNIT_TEST_FILES=${UNIT_TEST_FILES})

add_executable(unit-tests ${UNIT_TEST_FILES})
target_include_directories(unit-tests PUBLIC
        "${PROJECT_SOURCE_DIR}/tests/unit"
        ${ANTLR4_INCLUDE_DIRS}
        ${ANTLR_Lexer_OUTPUT_DIR}
        ${ANTLR_Parser_OUTPUT_DIR})
target_compile_definitions(unit-tests PUBLIC FIXTURES_PATH="${PROJECT_SOURCE_DIR}/tests/unit/Fixtures")

target_link_libraries(unit-tests PRIVATE gtest_main gtest gmock filc_lib)

gtest_discover_tests(unit-tests
        PROPERTIES LABELS "unit"
)

# _.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-.
# e2e tests

find_program(LLI_EXECUTABLE lli REQUIRED)
message(STATUS "Found lli: ${LLI_EXECUTABLE}")

file(GLOB_RECURSE E2E_TEST_FILES
        "${PROJECT_SOURCE_DIR}/tests/e2e/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/e2e/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/e2e/*.h"
        "${PROJECT_SOURCE_DIR}/tests/e2e/**/*.h"
)
message(DEBUG E2E_TEST_FILES=${E2E_TEST_FILES})

add_executable(e2e-tests ${E2E_TEST_FILES})
add_dependencies(e2e-tests filc)
target_include_directories(e2e-tests PUBLIC e2e)
target_compile_definitions(e2e-tests PUBLIC
        FILC_BIN="$<TARGET_FILE:filc>"
        FILC_VERSION="${FILC_VERSION}"
        FIXTURES_PATH="${PROJECT_SOURCE_DIR}/tests/unit/Fixtures"
        LLI_BIN="${LLI_EXECUTABLE}"
)

target_link_libraries(e2e-tests PRIVATE gtest_main gtest gmock)

gtest_discover_tests(e2e-tests
        PROPERTIES LABELS "e2e"
)
