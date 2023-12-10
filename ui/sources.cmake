get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(objects_name ${library_name}_object)

set(library_src
    include/${library_name}/canvas.h
    include/${library_name}/ui.h

    src/ui.cpp
)

set(object_library_src
    src/buffer.cpp
    src/buffer.h
    src/canvas.cpp
    src/command_buffer.cpp
    src/command_buffer.h
    src/device.cpp
    src/device.h
    src/index_buffer.cpp
    src/index_buffer.h
    src/renderer.cpp
    src/renderer.h
    src/ui.h
    src/utils.h
    src/vertex_buffer.cpp
    src/vertex_buffer.h
)

# Trick to show the file sources.cmake in the IDE
list(APPEND library_src sources.cmake)
set_source_files_properties(sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
