#pragma once

#include "label.hpp"
#include "widget.hpp"
#include <functional>
#include <spdlog/spdlog.h>

namespace ui {
    class Button : public Widget {
    private:
        Label m_caption;
        std::function<void(Button&)> m_on_click;
        bool m_hover_is_active{ false };
        bool m_click_started{ false };

    public:
        Button(std::string caption, std::shared_ptr<gfx::Font> font, std::function<void(Button&)> on_click)
            : m_caption{ std::move(caption),  std::move(font),   40,
                         utils::Color::Black, Alignment::Center, VerticalAlignment::Middle },
              m_on_click{ std::move(on_click) } { }

        [[nodiscard]] bool handle_event(Event const event) override {
            if (auto const mouse_moved = std::get_if<MouseMoved>(&event)) {
                m_hover_is_active = area().contains(mouse_moved->position);
            } else if (auto const mouse_clicked = std::get_if<MouseClicked>(&event)) {
                if (mouse_clicked->button == MouseButton::Left and area().contains(mouse_clicked->position)) {
                    m_click_started = true;
                }
            } else if (auto const mouse_released = std::get_if<MouseReleased>(&event)) {
                if (m_click_started and mouse_released->button == MouseButton::Left) {
                    if (area().contains(mouse_released->position) and m_on_click) {
                        m_on_click(*this);
                    }
                    m_click_started = false;
                }
            }
            return false;
        }

        void render(gfx::Renderer& renderer) const override {
            auto const color = std::invoke([&]() {
                if (m_click_started and m_hover_is_active) {
                    return utils::Color::DarkBlue;
                }
                if (m_click_started or m_hover_is_active) {
                    return utils::Color::SkyBlue;
                }
                return utils::Color::LightGray;
            });
            auto const background_area = area().move({ 1, 0 }).shrink({ 1, 1 });
            renderer.draw_filled_rounded_rectangle(background_area, 0.05f, 4, color);
            auto const outline_area = area().move({ 2, 1 }).shrink({ 3, 3 });
            renderer.draw_rounded_rectangle_outline(outline_area, 0.05f, 4, 1, utils::Color::Black);
            m_caption.render(renderer);
        }

        void recalculate_absolute_area(utils::IntRect const& outer_area) override {
            Widget::recalculate_absolute_area(outer_area);
            auto const label_area = area().scaled_from_center(0.9f);
            m_caption.recalculate_absolute_area(label_area);
        }

        [[nodiscard]] std::string const& caption() const {
            return m_caption.caption();
        }

        void caption(std::string caption) {
            m_caption.caption(std::move(caption));
        }
    };
} // namespace ui
