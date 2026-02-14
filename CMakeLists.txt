cmake_minimum_required(VERSION 3.16)

project(simgen VERSION 1.0 LANGUAGES CXX)

# -----------------------
# C++ Standard
# -----------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------
# Sources
# -----------------------
file(GLOB_RECURSE SIMGEN_SOURCES
    src/*.cpp
)

add_executable(simgen ${SIMGEN_SOURCES})

# -----------------------
# Include directories
# -----------------------
target_include_directories(simgen
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

# -----------------------
# Compiler warnings
# -----------------------
if(MSVC)
    target_compile_options(simgen PRIVATE /W4)
else()
    target_compile_options(simgen PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )
endif()

# -----------------------
# Optimization (Release)
# -----------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------
# Threading
# -----------------------
find_package(Threads REQUIRED)
target_link_libraries(simgen PRIVATE Threads::Threads)

# -----------------------
# TBB (Optional)
# -----------------------
find_package(TBB QUIET)

if(TBB_FOUND)
    message(STATUS "TBB found — enabling task-based parallelism")
    target_compile_definitions(simgen PRIVATE USE_TBB)
    target_link_libraries(simgen PRIVATE TBB::tbb)
else()
    message(WARNING "TBB not found — falling back to std::thread")
endif()
