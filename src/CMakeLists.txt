project(src)

set(HEADER_FILES
    eval.h
    macro.h
    math.h
    operation.h
    stack.h
    types.h)

set(SOURCE_FILES
    eval.cpp
    macro.cpp
    math.cpp
    stack.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
