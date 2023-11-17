include(sources.cmake REQUIRED)

file(DOWNLOAD https://github.com/mariusbancila/stduuid/releases/download/v1.2.3/uuid.h ${CMAKE_BINARY_DIR}/stduuid/uuid)

add_library(${library_name} SHARED ${library_src})

source_group("res" REGULAR_EXPRESSION ".*")
source_group("src" REGULAR_EXPRESSION ".*\\.(cpp|h|inl)")
source_group("include" REGULAR_EXPRESSION "include/${library_name}/.*")

generate_export_header(${library_name} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${library_name}_export.h)

target_include_directories(${library_name}
PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${library_name}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_BINARY_DIR}/stduuid
)

target_link_libraries(${library_name}
PUBLIC

PRIVATE
    uuid
)

add_subdirectory(test)
