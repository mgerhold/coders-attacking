#pragma once

#include "components.hpp"
#include "uuid.hpp"
#include <stdexcept>
#include <tl/optional.hpp>
#include <vector>

class GameObject final {
private:
    uuids::uuid m_uuid;
    std::string m_name;
    std::vector<Component> m_components;

    static inline std::size_t s_name_generator_counter{ 0 };

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameObject, m_uuid, m_name, m_components);

public:
    GameObject() : GameObject{ next_name() } { }
    explicit GameObject(std::string name) : GameObject{ generate_uuid(), std::move(name) } { }
    explicit GameObject(uuids::uuid const uuid) : GameObject{ uuid, next_name() } { }
    explicit GameObject(uuids::uuid const uuid, std::string name) : m_uuid{ uuid }, m_name{ std::move(name) } { }

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

    template<IsComponent C>
    [[nodiscard]] tl::optional<C&> get_component() {
        static_assert(std::same_as<C, std::decay_t<C>>);
        for (auto& component : m_components) {
            if (std::holds_alternative<C>(component)) {
                return std::get<C>(component);
            }
        }
        return tl::nullopt;
    }

    [[nodiscard]] uuids::uuid const& uuid() const {
        return m_uuid;
    }

    [[nodiscard]] std::string const& name() const {
        return m_name;
    }

    [[nodiscard]] bool operator==(GameObject const& other) const {
        return m_uuid == other.uuid();
    }

private:
    [[nodiscard]] static std::string next_name() {
        return std::format("GameObject{}", s_name_generator_counter++);
    }
};
