#pragma once

#include "event.hpp"
#include <gfx/renderer.hpp>
#include <utils/rect.hpp>
#include <utils/vec2.hpp>

namespace ui {
    class Widget {
    private:
        utils::IntRect m_absolute_area;

    protected:
        [[nodiscard]] utils::IntRect absolute_area() const {
            return m_absolute_area;
        }

    public:
        Widget() = default;
        Widget(Widget const& other) = default;
        Widget(Widget&& other) noexcept = default;
        Widget& operator=(Widget const& other) = default;
        Widget& operator=(Widget&& other) noexcept = default;
        virtual ~Widget() = default;

        [[nodiscard]] virtual bool handle_event(Event event) = 0;
        virtual void render(gfx::Renderer& renderer) const = 0;
        virtual void recalculate_absolute_area(utils::IntRect const& outer_area);
    };
} // namespace ui
