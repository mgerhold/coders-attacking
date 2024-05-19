add_library(view STATIC
        include/view/view.hpp
        view.cpp
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
