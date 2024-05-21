#pragma once

#include "utils/rect.hpp"


#include <algorithm>
#include <spdlog/spdlog.h>
#include <utils/vec2.hpp>

namespace view {
    class Camera final {
    private:
        utils::Vec2f m_offset;
        float m_zoom = 1.0f;
        float m_min_zoom;
        float m_max_zoom;
        utils::IntRect m_viewport;
        utils::FloatRect m_boundaries;

    public:
        Camera(float const min_zoom,
               float const max_zoom,
               utils::IntRect const viewport,
               utils::FloatRect const boundaries)
            : m_min_zoom{ min_zoom },
              m_max_zoom{ max_zoom },
              m_viewport{ viewport },
              m_boundaries{ boundaries } {
            if (max_zoom < min_zoom) {
                throw std::invalid_argument{ "max_zoom cannot be less then min_zoom" };
            }
        }

        void set_viewport(utils::IntRect const viewport) {
            m_viewport = viewport;
        }

        [[nodiscard]] utils::IntRect viewport() const {
            return m_viewport;
        }

        [[nodiscard]] utils::FloatRect visible_rect() const {
            auto const top_left = view_to_world_coords(utils::Vec2f{});
            auto const bottom_right = view_to_world_coords(utils::Vec2f{ 1.0f, 1.0f });
            auto const size = bottom_right - top_left;
            return utils::FloatRect{ top_left, size };
        }

        void move(utils::Vec2f const offset) {
            m_offset += offset;
            clamp_inside_boundaries();
        }

        void set_to_position(utils::Vec2f const position) {
            m_offset = position;
            clamp_inside_boundaries();
        }

        void reset_position() {
            set_to_position(utils::Vec2f{});
            clamp_inside_boundaries();
        }

        void zoom(float const factor) {
            zoom_unclamped(factor);
            clamp_inside_boundaries();
        }

        void zoom_towards(float const factor, utils::Vec2i const screen_coords) {
            auto const offset_from_center =
                    screen_to_view_coords(screen_coords) - screen_to_view_coords(viewport().center());
            m_offset += offset_from_center;
            auto const old_zoom = m_zoom;
            zoom_unclamped(factor);
            if (old_zoom == m_zoom) {
                m_offset -= offset_from_center;
            } else {
                m_offset -= offset_from_center / factor;
            }
            clamp_inside_boundaries();
        }

        void set_zoom(float const value) {
            auto const new_zoom = std::clamp(value, m_min_zoom, m_max_zoom);
            auto const factor = new_zoom / m_zoom;
            m_zoom = new_zoom;
            m_offset *= factor;
            clamp_inside_boundaries();
        }

        void reset_zoom() {
            set_zoom(1.0f);
            clamp_inside_boundaries();
        }

        // clang-format off
        [[nodiscard]] utils::Vec2f world_to_view_coords(
            utils::Vec2f world_coords,
            float parallax_distance = 1.0f
        ) const;
        // clang-format on

        [[nodiscard]] utils::Vec2i view_to_screen_coords(utils::Vec2f view_coords) const;

        // clang-format off
        [[nodiscard]] utils::Vec2i world_to_screen_coords(
            utils::Vec2f world_coords,
            float parallax_distance = 1.0f
        ) const;
        // clang-format on

        [[nodiscard]] utils::Vec2f screen_to_view_coords(utils::Vec2i screen_coords) const;

        [[nodiscard]] utils::Vec2f view_to_world_coords(utils::Vec2f view_coords) const;

        [[nodiscard]] utils::Vec2f screen_to_world_coords(utils::Vec2i screen_coords) const;

    private:
        void zoom_unclamped(float const factor) {
            auto const new_zoom = std::clamp(m_zoom * factor, m_min_zoom, m_max_zoom);
            auto const actual_factor = new_zoom / m_zoom;
            m_zoom = new_zoom;
            m_offset *= actual_factor;
        }

        void clamp_inside_boundaries() {
            auto const visible = visible_rect();
            if (auto const x_offset = m_boundaries.top_left.x - visible.top_left.x; x_offset > 0.0f) {
                m_offset.x += x_offset;
            }
            if (auto const x_offset = visible.bottom_right().x - m_boundaries.bottom_right().x; x_offset > 0.0f) {
                m_offset.x -= x_offset;
            }
            if (auto const y_offset = m_boundaries.top_left.y - visible.top_left.y; y_offset > 0.0f) {
                m_offset.y += y_offset;
            }
            if (auto const y_offset = visible.bottom_right().y - m_boundaries.bottom_right().y; y_offset > 0.0f) {
                m_offset.y -= y_offset;
            }
        }
    };
} // namespace view
