if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(arg)
add_subdirectory(as)
add_subdirectory(e)
add_subdirectory(evening)
add_subdirectory(tempo)
add_subdirectory(thing)
add_subdirectory(ve)

add_subdirectory(slash)
add_subdirectory(re)
