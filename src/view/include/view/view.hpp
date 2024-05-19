#pragma once

#include "gfx/renderer.hpp"
#include "ui/event.hpp"
#include "utils/rect.hpp"
#include <game/galaxy.hpp>
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

            BackgroundStar(
                    utils::Vec2f position,
                    float const size,
                    float const base_brightness,
                    float const period,
                    float const amplitude
            )
                : position{ position },
                  size{ size },
                  base_brightness{ base_brightness },
                  period{ period },
                  amplitude{ amplitude } { }
        };

        // float m_zoom = 1.0f;
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

        void update(float delta_seconds);
    };


} // namespace view
