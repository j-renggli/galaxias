get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(library_name ${library_name} NAME)
set(test_name test_${library_name})

set(library_src
    planet.cpp
    quantities.cpp
    star.cpp
    system.cpp
    system_identifier.cpp
)

add_executable(${test_name} ${library_src})

source_group("res" REGULAR_EXPRESSION ".*")
source_group("src" REGULAR_EXPRESSION ".*\\.(cpp|h|inl)")
source_group("include" REGULAR_EXPRESSION "include/${library_name}/.*")

target_include_directories(${test_name}
PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${library_name}/include>
PRIVATE
    $<TARGET_PROPERTY:orbit,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:math,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${test_name}
    orbit
    system
    Catch2::Catch2WithMain
)
