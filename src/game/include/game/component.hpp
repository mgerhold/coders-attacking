#pragma once

#include "planet.hpp"
#include "transform.hpp"
#include <lib2k/concepts.hpp>
#include <variant>

using Component = std::variant<Planet, Transform>;

template<typename T>
concept IsComponent = c2k::IsOneOf<Planet, Transform>;
