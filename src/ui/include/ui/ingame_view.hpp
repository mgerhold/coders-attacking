#pragma once

#include "widget.hpp"

namespace ui {
    class IngameView : public Widget {
    public:
        [[nodiscard]] bool handle_event(Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override {
            renderer.draw_filled_rectangle(area(), utils::Color::Black);
        }
    };
} // namespace ui
