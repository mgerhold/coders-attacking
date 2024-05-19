#pragma once

#include "widget.hpp"
#include <view/view.hpp>

namespace ui {
    class IngameView : public Widget {
    private:
        Galaxy const* m_galaxy;
        gfx::Font const* m_font;

    public:
        explicit IngameView(Galaxy const& galaxy, gfx::Font const& font) : m_galaxy{ &galaxy }, m_font{ &font } { }

        [[nodiscard]] bool handle_event(Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override {
            view::render_game(*m_galaxy, area(), renderer, *m_font);
        }
    };
} // namespace ui
