#pragma once

#include "focus_manager.hpp"
#include "label.hpp"
#include "widget.hpp"
#include <functional>
#include <spdlog/spdlog.h>

namespace ui {
    class Button final : public Widget {
    private:
        Label m_caption;
        std::function<void(Button&)> m_on_click;
        bool m_hover_is_active{ false };
        bool m_click_started{ false };
        bool m_keypress_started{ false };
        u32 m_focus_id;

    public:
        Button(std::string caption, u32 focus_id, gfx::Font const& font, std::function<void(Button&)> on_click = {});

        Button(WidgetName&& widget_name,
               std::string caption,
               u32 focus_id,
               gfx::Font const& font,
               std::function<void(Button&)> on_click = {});

        [[nodiscard]] HandleEventResult handle_event(Event event) override;

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override;

        [[nodiscard]] std::string const& caption() const {
            return m_caption.caption();
        }

        void caption(std::string caption) {
            m_caption.caption(std::move(caption));
        }

        void collect_focusable_widgets(std::vector<Widget*>& focusable_widgets) override {
            focusable_widgets.push_back(this);
        }

        void on_click(std::function<void(Button&)> function) {
            m_on_click = std::move(function);
        }

        [[nodiscard]] tl::optional<u32> focus_id() const override {
            return m_focus_id;
        }
    };
} // namespace ui
