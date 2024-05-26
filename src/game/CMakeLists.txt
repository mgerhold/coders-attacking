add_library(game STATIC
        include/game/components.hpp
        include/game/game_object.hpp
        include/game/galaxy.hpp
        include/game/uuid.hpp
        uuid.cpp
        galaxy.cpp
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

if(UNIX)
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(UUID REQUIRED uuid)
    target_link_libraries(coders_attacking PRIVATE ${UUID_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
