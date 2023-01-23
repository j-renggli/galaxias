project(galaxias)

cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -g
    -pedantic
    -Wall
    -Wextra
    -fPIC
)

include(GenerateExportHeader)

add_subdirectory(orbit)

set(library_src
    main.cpp
    .clang-format
)

set_source_files_properties(.clang-format PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(galaxias ${library_src})

target_link_libraries(galaxias
    orbit
)

source_group("res" REGULAR_EXPRESSION ".*")
source_group("src" REGULAR_EXPRESSION ".*\\.(cpp|h|inl)")
