add_custom_command(
    COMMENT "generating re_generated.h from re.fbs"
    COMMAND $<TARGET_FILE:flatc>
        --cpp
        --scoped-enums
        --cpp-field-case-style lower
        --cpp-std c++17
        -o "${CMAKE_CURRENT_BINARY_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/re.fbs"
    DEPENDS re.fbs
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/re_generated.h"
)

add_library(re
    sprites.cpp
    util.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/include/re_generated.h"
)
target_include_directories(re PUBLIC
    include
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)
target_link_libraries(re PRIVATE flatbuffers)

add_subdirectory(pack)
