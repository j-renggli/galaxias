get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(objects_name ${library_name}_object)

set(library_src
    include/${library_name}/quantity.h
    include/${library_name}/range.h
    include/${library_name}/range.inl
    include/${library_name}/unit.h

    include/${library_name}/rng/mersenne.h

    include/${library_name}/solver/bisection.h
    include/${library_name}/solver/brent.h
    include/${library_name}/solver/function.h
    include/${library_name}/solver/solver.h

    src/rng/mersenne.cpp

    src/solver/bisection.cpp
    src/solver/brent.cpp
)

# Trick to show the file sources.cmake in the IDE
list(APPEND library_src sources.cmake)
set_source_files_properties(sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
