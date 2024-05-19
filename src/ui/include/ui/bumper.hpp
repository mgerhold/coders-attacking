#pragma once

#include "widget.hpp"

namespace ui {
    class Bumper : public Widget {
    public:
        [[nodiscard]] bool handle_event(Event) override {
            return false;
        }

        void render(gfx::Renderer&) const override { }
    };
} // namespace ui
