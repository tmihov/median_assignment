find_package(spdlog)

# Generic test that uses conan libs
add_executable(median_assignment main.cpp)

add_library(median median.cpp median.hpp)

target_link_libraries(
    median
    PRIVATE spdlog::spdlog)

target_link_libraries(
  median_assignment
  PRIVATE project_options
          project_warnings
          spdlog::spdlog
          median)
