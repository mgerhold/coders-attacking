#pragma once

#include "component.hpp"
#include <stdexcept>
#include <tl/optional.hpp>
#include <vector>

class GameObject final {
private:
    std::vector<Component> m_components;

public:
    void add_component(IsComponent auto&& component) {
        emplace_component<std::decay_t<decltype(component)>>(std::forward<decltype(component)>(component));
    }

    template<IsComponent C>
    void emplace_component(auto&&... args) {
        static_assert(std::same_as<C, std::decay_t<C>>);
        if (get_component<C>().has_value()) {
            throw std::runtime_error{ "duplicate components are not allowed" };
        }
        m_components.emplace_back(std::in_place_type<C>, std::forward<decltype(args)>(args)...);
    }

    template<IsComponent C>
    [[nodiscard]] tl::optional<C const&> get_component() const {
        static_assert(std::same_as<C, std::decay_t<C>>);
        for (auto const& component : m_components) {
            if (std::holds_alternative<C>(component)) {
                return std::get<C>(component);
            }
        }
        return tl::nullopt;
    }
};
