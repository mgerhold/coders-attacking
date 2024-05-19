add_executable(coders_attacking
        main.cpp
)

target_link_libraries(coders_attacking
        PRIVATE
        game
        gfx
)

target_link_system_libraries(coders_attacking
        PRIVATE
        spdlog::spdlog
)
