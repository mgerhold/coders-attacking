#pragma once

#include "background_star.hpp"
#include "view/camera.hpp"
#include <game/galaxy.hpp>
#include <gfx/renderer.hpp>
#include <ui/event.hpp>
#include <ui/event_system.hpp>
#include <utils/rect.hpp>
#include <vector>
#include <common/service_provider.hpp>

namespace view {
    class View final {
    private:
        Camera m_camera;
        std::vector<BackgroundStar> m_background_stars;
        tl::optional<GameObject const&> m_focused_planet;
        ServiceProvider* m_service_provider;

    public:
        explicit View(ServiceProvider& service_provider);

        void render_game(Galaxy const& galaxy, gfx::Renderer& renderer) const;

        [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event);

        void update(Galaxy const& galaxy);

        void on_window_resized(utils::IntRect const viewport) {
            m_camera.set_viewport(viewport);
        }

        [[nodiscard]] tl::optional<GameObject const&> focused_planet() const {
            return m_focused_planet;
        }
    };


} // namespace view
