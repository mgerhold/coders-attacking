#pragma once

#include "event.hpp"
#include <utils/rect.hpp>
#include <utils/vec2.hpp>
#include <gfx/renderer.hpp>

namespace ui {
    class Widget {
    protected:
        utils::FloatRect m_relative_area;
        utils::IntRect m_absolute_area;

    public:
        explicit Widget(utils::FloatRect const& relative_area) : m_relative_area{ relative_area } { }
        Widget(Widget const& other) = default;
        Widget(Widget&& other) noexcept = default;
        Widget& operator=(Widget const& other) = default;
        Widget& operator=(Widget&& other) noexcept = default;
        virtual ~Widget() = default;

        [[nodiscard]] virtual bool handle_event(Event event) = 0;
        virtual void update() { }
        virtual void render(gfx::Renderer& renderer) const = 0;
        virtual void recalculate_absolute_area(utils::IntRect const& outer_area);
    };
} // namespace ui
