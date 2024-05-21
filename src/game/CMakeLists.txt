add_library(game INTERFACE
        include/game/components.hpp
        include/game/game_object.hpp
        include/game/galaxy.hpp
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
        spdlog::spdlog
        stduuid
        nlohmann_json::nlohmann_json
)

target_include_directories(game
        INTERFACE
        include
)
