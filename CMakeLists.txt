cmake_minimum_required(VERSION 3.22)
project(FluidLab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(raylib)

add_executable(FluidLab
  src/main.cpp
  src/SPHSolver.cpp
  src/CacheWriter.cpp)
target_include_directories(FluidLab PRIVATE src)
target_link_libraries(FluidLab PRIVATE raylib)

if(MSVC)
  target_compile_options(FluidLab PRIVATE /W4 /permissive-)
else()
  target_compile_options(FluidLab PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS FluidLab RUNTIME DESTINATION .)

