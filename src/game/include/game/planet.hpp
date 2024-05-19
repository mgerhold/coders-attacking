#pragma once

#include "utils/color.hpp"


#include <lib2k/types.hpp>
#include <string>

struct Planet {
    std::string name;
    usize production_per_turn;
    utils::Color color;
};
