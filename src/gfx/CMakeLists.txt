add_library(gfx STATIC
        include/gfx/window.hpp
        window.cpp
        include/gfx/renderer.hpp
        renderer.cpp
        include/gfx/font.hpp
        font.cpp
)

target_include_directories(gfx
        PUBLIC
        include
)

target_link_libraries(gfx
        PRIVATE
        coders_attacking_options
        PUBLIC
        game
        lib2k
        ui
)

target_link_system_libraries(gfx
        PRIVATE
        raylib
)

if (WIN32)
    target_link_system_libraries(gfx PRIVATE Winmm)
endif ()
