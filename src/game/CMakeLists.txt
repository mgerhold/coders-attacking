add_library(game INTERFACE
        include/game/component.hpp
        include/game/game_object.hpp
        include/game/planet.hpp
        include/game/galaxy.hpp
        include/game/transform.hpp
)

target_link_libraries(game
        INTERFACE
        utils
)

target_link_system_libraries(game
        INTERFACE
        Microsoft.GSL::GSL
        tl::expected
        tl::optional
        lib2k
)

target_include_directories(game
        INTERFACE
        include
)
