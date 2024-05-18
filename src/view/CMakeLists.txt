add_library(view STATIC
        include/view/scene.hpp
        scene.cpp
        include/view/window.hpp
        window.cpp
        include/view/renderer.hpp
        renderer.cpp
)

target_include_directories(view
        PUBLIC
        include
)

target_link_libraries(view
        PRIVATE
        coders_attacking_options
        PUBLIC
        game
        lib2k
        ui
)

target_link_system_libraries(view
        PRIVATE
        raylib
)

if (WIN32)
    target_link_system_libraries(view PRIVATE Winmm)
endif ()
