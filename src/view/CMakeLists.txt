add_library(view STATIC
        include/view/view.hpp
        view.cpp
        include/view/camera.hpp
        camera.cpp
        include/view/background_star.hpp
)

target_include_directories(view
        PUBLIC
        include
)

target_link_libraries(view
        PRIVATE
        coders_attacking_options
        PUBLIC
        gfx
        game
        utils
)
