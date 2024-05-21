add_executable(coders_attacking
        scenes/test_scene.hpp
        main.cpp
        scene.hpp
        scene_manager.hpp
        scene_stack.cpp
        scene_stack.hpp
        scenes/test_scene.cpp
)

target_link_libraries(coders_attacking
        PRIVATE
        game
        gfx
        ui
        utils
)

target_link_system_libraries(coders_attacking
        PRIVATE
        spdlog::spdlog
)
