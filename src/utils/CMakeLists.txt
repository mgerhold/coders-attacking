add_library(utils INTERFACE
    include/utils/vec2.hpp
        include/utils/color.hpp
        include/utils/rect.hpp
        include/utils/concepts.hpp
        include/utils/math.hpp
)

target_include_directories(utils INTERFACE include)
