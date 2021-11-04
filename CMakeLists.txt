cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name, my project isn't very descriptive
project(median_assignment CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_14)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

option(ENABLE_CONAN "Use Conan for dependency management" ON)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")

  find_package(Catch2 REQUIRED)

  include(CTest)
  include(Catch)

  add_library(catch_main STATIC test/catch_main.cpp)
  target_link_libraries(catch_main PUBLIC Catch2::Catch2)
  target_link_libraries(catch_main PRIVATE project_options)

  add_executable(tests test/tests.cpp)
  target_link_libraries(tests PRIVATE project_warnings project_options catch_main median)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
  catch_discover_tests(
    tests
    TEST_PREFIX
    "unittests."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "unittests."
    OUTPUT_SUFFIX
    .xml)

# Add a file containing a set of constexpr tests
  add_executable(constexpr_tests test/constexpr_tests.cpp)
  target_link_libraries(constexpr_tests PRIVATE project_options project_warnings catch_main)

  catch_discover_tests(
    constexpr_tests
    TEST_PREFIX
    "constexpr."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "constexpr."
    OUTPUT_SUFFIX
    .xml)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
  add_executable(relaxed_constexpr_tests test/constexpr_tests.cpp)
  target_link_libraries(relaxed_constexpr_tests PRIVATE project_options project_warnings catch_main)
  target_compile_definitions(relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)

  catch_discover_tests(
    relaxed_constexpr_tests
    TEST_PREFIX
    "relaxed_constexpr."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "relaxed_constexpr."
    OUTPUT_SUFFIX
    .xml)
endif()

if(ENABLE_FUZZING)
  message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
  add_subdirectory(fuzz_test)
endif()

add_subdirectory(src)

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(intro PROPERTIES UNITY_BUILD ON)
endif()
