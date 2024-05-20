#pragma once

#include "utils/rect.hpp"


#include <algorithm>
#include <utils/vec2.hpp>

namespace view {
    class Camera final {
    private:
        utils::Vec2f m_offset;
        float m_zoom = 1.0f;
        float m_min_zoom;
        float m_max_zoom;

    public:
        Camera(float const min_zoom, float const max_zoom) : m_min_zoom{ min_zoom }, m_max_zoom{ max_zoom } {
            if (max_zoom < min_zoom) {
                throw std::invalid_argument{ "max_zoom cannot be less then min_zoom" };
            }
        }

        void move(utils::Vec2f const offset) {
            m_offset += offset;
        }

        void set_to_position(utils::Vec2f const position) {
            m_offset = position;
        }

        void reset_position() {
            set_to_position(utils::Vec2f{});
        }

        void zoom(float const factor) {
            auto const new_zoom = std::clamp(m_zoom * factor, m_min_zoom, m_max_zoom);
            auto const actual_factor = new_zoom / m_zoom;
            m_zoom = new_zoom;
            m_offset *= actual_factor;
        }

        void set_zoom(float const value) {
            auto const new_zoom = std::clamp(value, m_min_zoom, m_max_zoom);
            auto const factor = new_zoom / m_zoom;
            m_zoom = new_zoom;
            m_offset *= factor;
        }

        void reset_zoom() {
            set_zoom(1.0f);
        }

        // clang-format off
        [[nodiscard]] utils::Vec2f world_to_view_coords(
            utils::Vec2f world_coords,
            float parallax_distance = 1.0f
        ) const;
        // clang-format off
        [[nodiscard]] utils::FloatRect world_to_view_coords(utils::FloatRect world_coords) const;
        [[nodiscard]] utils::Vec2i view_to_screen_coords(utils::Vec2f view_coords, utils::IntRect viewport) const;
        [[nodiscard]] utils::IntRect view_to_screen_coords(utils::FloatRect view_coords, utils::IntRect viewport) const;
        [[nodiscard]] utils::Vec2i world_to_screen_coords(
            utils::Vec2f world_coords,
            utils::IntRect viewport,
            float parallax_distance = 1.0f
        ) const;
        [[nodiscard]] utils::IntRect world_to_screen_coords(
            utils::FloatRect world_coords,
            utils::IntRect viewport
        ) const;
        // clang-format off
    };
} // namespace view
