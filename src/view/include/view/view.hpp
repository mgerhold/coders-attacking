#pragma once

#include "background_star.hpp"
#include "view/camera.hpp"
#include <game/galaxy.hpp>
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/event_system.hpp>
#include <utils/rect.hpp>
#include <vector>

namespace view {
    class View final {
    private:
        Camera m_camera;
        std::vector<BackgroundStar> m_background_stars;
        float m_elapsed_time = 0.0f;

    public:
        explicit View(utils::IntRect viewport);

        void render_game(Galaxy const& galaxy, gfx::Renderer& renderer, gfx::Font const& font) const;

        [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event);

        void update(ui::EventSystem const& event_system, float delta_seconds);

        void on_window_resized(utils::IntRect const viewport) {
            m_camera.set_viewport(viewport);
        }
    };


} // namespace view
