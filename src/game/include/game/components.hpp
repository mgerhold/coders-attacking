#pragma once

#include "uuid.hpp"
#include <format>
#include <lib2k/concepts.hpp>
#include <lib2k/types.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <utils/color.hpp>
#include <utils/vec2.hpp>
#include <variant>

struct Player final {
    static constexpr auto type = "Player";
    std::string name;
    utils::Color color;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Player, name, color);

struct Ownership final {
    static constexpr auto type = "Ownership";
    uuids::uuid owner;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Ownership, owner);

struct Planet final {
    static constexpr auto type = "Planet";
    std::string name;
    usize production_per_turn{ 0 };
    utils::Color color;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Planet, name, production_per_turn, color);

struct Transform final {
    static constexpr auto type = "Transform";
    utils::Vec2f position;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, position);

using Component = std::variant<Player, Ownership, Planet, Transform>;

template<typename T>
concept IsComponent = c2k::IsOneOf<T, Player, Ownership, Planet, Transform>;

template<typename... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template<typename T, typename Visitor>
auto visit(T t, Visitor v) {
    return std::visit(v, t);
}

inline void from_json(nlohmann::json const& j, Component& component) {
    auto const& type = j.at("type");
    if (type == Player::type) {
        component = j.get<Player>();
    } else if (type == Ownership::type) {
        component = j.get<Ownership>();
    } else if (type == Planet::type) {
        component = j.get<Planet>();
    } else if (type == Transform::type) {
        component = j.get<Transform>();
    } else {
        throw std::runtime_error{ std::format("'{}' is not a known component type", type.get<std::string>()) };
    }
}

inline void to_json(nlohmann::json& j, Component const& component) {
    // clang-format off
    using json = nlohmann::json;

    // first put the actual data into the JSON object...
    auto result = visit(
        component,
        Overloaded{
            [&](auto const& alternative) -> json {
                return alternative;
            },
        }
    );

    // ...then also add the type tag
    visit(
        component,
        Overloaded{
            [&]<typename C>(C const&) {
                result["type"] = std::string{C::type};
            }
        }
    );
    j = std::move(result);

    // clang-format on
}
