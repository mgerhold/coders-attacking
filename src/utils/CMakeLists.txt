add_library(utils INTERFACE
    include/utils/vec2.hpp
        include/utils/color.hpp
)

target_include_directories(utils INTERFACE include)
