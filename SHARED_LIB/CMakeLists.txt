cmake_minimum_required(VERSION 3.10)
add_library(mul STATIC include/multiply.cpp)
add_library(physics STATIC src/phsyics.cpp)
target_link_libraries(physics PUBLIC mul)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/output.txt
    COMMAND ${CMAKE_COMMAND} -E echo "Something important ifo or state of lib " > ${CMAKE_CURRENT_BINARY_DIR}/output.txt
    COMMENT "Gnerating simulation"
    VERBATIM
)
add_custom_target(wait DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/output.txt)
add_executable(phys_sim src/main.cpp)
target_link_libraries(phys_sim PRIVATE physics)
add_dependencies(phys_sim wait)

add_custom_command(
    TARGET phys_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "------BUILD SUCCESSFUL-------"
    COMMAND ${CMAKE_COMMAND} -E echo "Binary is ready $<TARGET_FILE:phys_sim>"
)