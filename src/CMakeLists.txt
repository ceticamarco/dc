project(src)

set(HEADER_FILES
        eval.h
        macro.h
        mathematics.h
        operation.h
        stack.h
        adt.h
)

set(SOURCE_FILES
        eval.cpp
        macro.cpp
        mathematics.cpp
        stack.cpp
        adt.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
