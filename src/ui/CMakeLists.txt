add_library(ui STATIC
        include/ui/widget.hpp
        include/ui/event_system.hpp
        event_system.cpp
        include/ui/key.hpp
        include/ui/event.hpp
        include/ui/mouse_button.hpp
        include/ui/label.hpp
        label.cpp
        widget.cpp
        include/ui/panel.hpp
        include/ui/layout.hpp
        panel.cpp
        include/ui/column_layout.hpp
        include/ui/button.hpp
        include/ui/grid_layout.hpp
        include/ui/bumper.hpp
        include/ui/alignment.hpp
)

target_include_directories(ui
        PUBLIC
        include
)

target_link_libraries(ui
        PUBLIC
        utils
        gfx
        view
        PRIVATE
        coders_attacking_options
)

target_link_system_libraries(ui
        PUBLIC
        tl::optional
        PRIVATE
        raylib
)
