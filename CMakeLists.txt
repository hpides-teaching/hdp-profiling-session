cmake_minimum_required(VERSION 3.18)
project(join_benchmark)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3 -gdwarf-4 -fno-omit-frame-pointer -march=native -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)
FetchContent_Declare(google_benchmark GIT_REPOSITORY https://github.com/google/benchmark.git GIT_TAG v1.8.0)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing google benchmark's tests" FORCE)
FetchContent_MakeAvailable(google_benchmark)


add_executable(join_benchmark join_benchmark.cpp)
target_link_libraries(join_benchmark benchmark)
target_compile_options(join_benchmark PRIVATE "-Wall" "-Wextra")
