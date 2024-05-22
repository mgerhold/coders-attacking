add_library(common STATIC
        include/common/service_provider.hpp
        include/common/resource_manager.hpp
        resource_manager.cpp
)

target_include_directories(common
        PUBLIC
        include
)
target_link_libraries(common
        PUBLIC
        gfx
)

target_link_system_libraries(common
        PRIVATE
        raylib
)
