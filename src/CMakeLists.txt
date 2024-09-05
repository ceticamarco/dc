project(src)

set(HEADER_FILES
        eval.h
        macro.h
        mathematics.h
        statistics.h
        bitwise.h
        operation.h
        stack.h
        adt.h
        num_utils.h
)

set(SOURCE_FILES
        eval.cpp
        macro.cpp
        mathematics.cpp
        statistics.cpp
        bitwise.cpp
        stack.cpp
        adt.cpp
        num_utils.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
