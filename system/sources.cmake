get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(objects_name ${library_name}_object)

set(library_src
    include/${library_name}/body.h
    include/${library_name}/system.h
    include/${library_name}/system_identifier.h
)

set(object_library_src
    src/system.cpp
    src/system_identifier.cpp

    src/bodies/planet.cpp
    src/bodies/planet.h
    src/bodies/star.cpp
    src/bodies/star.h

    src/quantity/earth.h
    src/quantity/galactic.h
    src/quantity/solar.h
)

# Trick to show the file sources.cmake in the IDE
list(APPEND library_src sources.cmake)
set_source_files_properties(sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
