#pragma once

#include "label.hpp"
#include "widget.hpp"
#include <functional>

namespace ui {
    class Button : public Widget {
    private:
        Label m_caption;
        std::function<void(Button&)> m_on_click;

    public:
        Button(std::string caption, std::shared_ptr<gfx::Font> font, std::function<void(Button&)> on_click)
            : m_caption{ std::move(caption),  std::move(font),   100,
                         utils::Color::Black, Alignment::Center, VerticalAlignment::Middle },
              m_on_click{ std::move(on_click) } { }

        [[nodiscard]] bool handle_event(Event event) override {
            // todo
            return false;
        }

        void render(gfx::Renderer& renderer) const override {
            renderer.draw_filled_rectangle(absolute_area(), utils::Color::LightGray);
            m_caption.render(renderer);
        }

        void recalculate_absolute_area(utils::IntRect const& outer_area) override {
            Widget::recalculate_absolute_area(outer_area);
            auto const label_area = absolute_area().scaled_from_center(0.9f);
            m_caption.recalculate_absolute_area(label_area);
        }
    };
} // namespace ui
