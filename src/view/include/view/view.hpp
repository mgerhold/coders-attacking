#pragma once

#include <game/galaxy.hpp>
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/event_system.hpp>
#include <utils/rect.hpp>
#include <vector>

namespace view {
    class View final {
    private:
        struct BackgroundStar final {
            utils::Vec2f position;
            float size;
            float base_brightness;
            float period;
            float amplitude;
            float distance;
            utils::Color color;

            BackgroundStar(
                    utils::Vec2f position,
                    float const size,
                    float const base_brightness,
                    float const period,
                    float const amplitude,
                    float const distance,
                    utils::Color const color
            )
                : position{ position },
                  size{ size },
                  base_brightness{ base_brightness },
                  period{ period },
                  amplitude{ amplitude },
                  distance{ distance },
                  color{ color } { }
        };

        float m_zoom = 1.0f;
        utils::Vec2f m_offset;
        std::vector<BackgroundStar> m_background_stars;
        float m_elapsed_time = 0.0f;

    public:
        View();

        void render_game(
                Galaxy const& galaxy,
                utils::IntRect const& viewport,
                gfx::Renderer& renderer,
                gfx::Font const& font
        ) const;

        [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event);

        void update(ui::EventSystem const& event_system, float delta_seconds);
    };


} // namespace view
