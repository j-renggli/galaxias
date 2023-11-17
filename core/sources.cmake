get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(objects_name ${library_name}_object)

set(library_src
    include/${library_name}/array.h
    include/${library_name}/array.inl
)

# Trick to show the file sources.cmake in the IDE
list(APPEND library_src sources.cmake)
set_source_files_properties(sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
