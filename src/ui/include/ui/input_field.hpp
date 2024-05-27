#pragma once

#include "widget.hpp"
#include <lib2k/utf8/string.hpp>
#include <ui/label.hpp>

namespace ui {
    class InputField final : public Widget {
    private:
        Label m_placeholder_label;
        Label m_text_label;
        c2k::Utf8String m_text;
        u32 m_focus_id;
        std::function<void(InputField&)> m_on_enter_pressed;

        static constexpr auto background_color = utils::Color::White;
        static constexpr auto placeholder_color = utils::Color::DarkGray;
        static constexpr auto regular_color = utils::Color::Black;

    public:
        InputField(std::string placeholder, u32 focus_id, gfx::Font const& font);
        InputField(WidgetName&& widget_name, std::string placeholder, u32 focus_id, gfx::Font const& font);

        void on_enter_pressed(std::function<void(InputField&)> function);

        [[nodiscard]] HandleEventResult handle_event(Event event) override;
        void render(gfx::Renderer& renderer) const override;
        [[nodiscard]] c2k::Utf8String const& text() const;
        void recalculate_absolute_area(utils::IntRect const& outer_area) override;
        [[nodiscard]] tl::optional<u32> focus_id() const override;
        void on_focused() override;
        void on_unfocused() override;
    };
} // namespace ui
