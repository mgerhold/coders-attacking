add_library(ui STATIC
        include/ui/widget.hpp
        include/ui/event_system.hpp
        event_system.cpp
        include/ui/key.hpp
        include/ui/event.hpp
        include/ui/mouse_button.hpp
)

target_include_directories(ui
        PUBLIC
        include
)

target_link_libraries(ui
        PUBLIC
        utils
        PRIVATE
        coders_attacking_options
)

target_link_system_libraries(ui
        PUBLIC
        tl::optional
        PRIVATE
        raylib
)
