add_library(plasma INTERFACE)
target_precompile_headers(plasma
    INTERFACE
    <vector>
    <array>
    <cmath>
    <cassert>
    <stdexcept>
    <string>
    <iostream>
    <fstream>
    <sstream>
    <algorithm>
    <numeric>
    <functional>
    <memory>
    <cstdint>
    <cststring>
)
add_library(plasma_core STATIC
    grid.cpp
    distribution.cpp
    interpolation.cpp
    poisson.cpp
    advection.cpp
    diagnostic.cpp
    solver.cpp
)
target_include_directories(plasma_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(plasma_core
    PRIVATE 
        plasma
)
target_precompile_headers(plasma_core
    PRIVATE
        "../include/plasma_type.h"
        "$<$<COMPILE_LANGUAGE:CXX>: <complex$<ANGLE-R>>"
        "$<$<COMPILE_LANGUAGE:CXX>:<limits$<ANGLE-R>>"
)
target_compile_definitions(plasma_core
    PRIVATE
        $<$<BOOL:${PLASMA_VERBOSE_OUTPUT}>:PLASMA_VERBOSE>
        PLASMA_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        PLASMA_VERSION_MINOR=${PROJECT_VERSION_MINOR}
)
target_compile_options(plasma_core
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -march=native -ffast-math -Wall -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:/02 /W4>
)
if(PLASMA_ENABLE_OPENMP AND OpenMP_CXX_FOUND)
    target_link_libraries(plasma_core
        PUBLIC
        OpenMP::OpenMP_CXX
    )
    target_compile_definitions(plasma_core PUBLIC PLASMA_OPENMP)
    message(STATUS "OpenMP enabled for plasma_core")
endif()
add_executable(plasma_solver
    main.cpp
)
target_link_libraries(plasma_solver PRIVATE plasma_core)
target_precompile_headers(plasma_solver
    REUSE_FROM
        plasma_core
)
message(STATUS "plasma_solver will reuse pch from plasma_core")
install(TARGETS plasma_solver RUNTIME DESTINATION bin)
install(TARGETS plasma_core ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/plasma)