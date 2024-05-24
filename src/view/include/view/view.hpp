#pragma once

#include "background_star.hpp"
#include "view/camera.hpp"
#include <common/service_provider.hpp>
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
        tl::optional<GameObject const&> m_focused_planet;
        ServiceProvider* m_service_provider;

        static constexpr auto num_background_stars = usize{ 1500 * 3 };

    public:
        explicit View(ServiceProvider& service_provider, c2k::Random::Seed background_stars_seed);

        void render_game(Galaxy const& galaxy, gfx::Renderer& renderer) const;

        [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event);

        void update(Galaxy const& galaxy);

        void on_window_resized(utils::IntRect const viewport) {
            m_camera.set_viewport(viewport);
        }

        [[nodiscard]] tl::optional<GameObject const&> focused_planet() const {
            return m_focused_planet;
        }

        void regenerate_background_stars(c2k::Random::Seed seed);

    private:
        [[nodiscard]] tl::optional<float> determine_visibility(
                float max_radius,
                Galaxy const& galaxy,
                uuids::uuid current_player_uuid,
                utils::Vec2f world_position
        ) const;
    };


} // namespace view
