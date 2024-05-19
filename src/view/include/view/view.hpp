#pragma once

#include "gfx/renderer.hpp"
#include "ui/event.hpp"
#include "utils/rect.hpp"
#include <game/galaxy.hpp>

namespace view {
    class View final {
    private:
        float m_zoom = 1.0f;

    public:
        void render_game(
                Galaxy const& galaxy,
                utils::IntRect const& viewport,
                gfx::Renderer& renderer,
                gfx::Font const& font
        ) const;

        [[nodiscard]] ui::HandleEventResult handle_event(ui::Event const& event);
    };


} // namespace view
