add_library(game STATIC
        include/game/components.hpp
        include/game/game_object.hpp
        include/game/galaxy.hpp
        include/game/uuid.hpp
        uuid.cpp
)

target_link_libraries(game
        PUBLIC
        utils
)

target_link_system_libraries(game
        PUBLIC
        lib2k
        nlohmann_json::nlohmann_json
        PRIVATE
        stduuid
)

target_include_directories(game
        PUBLIC
        include
)
