#pragma once

#include "widget.hpp"

namespace ui {
    class Bumper : public Widget {
    public:
        [[nodiscard]] HandleEventResult handle_event(Event) override {
            return HandleEventResult::EventNotHandled;
        }

        void render(gfx::Renderer&) const override { }
    };
} // namespace ui
