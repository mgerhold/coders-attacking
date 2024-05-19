#pragma once

#include "widget.hpp"
#include <view/view.hpp>

namespace ui {
    class IngameView : public Widget {
    public:
        [[nodiscard]] bool handle_event(Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override {
            auto galaxy = Galaxy{};
            view::render_game(galaxy, area(), renderer);
        }
    };
} // namespace ui
