#pragma once

#include "alignment.hpp"
#include "widget.hpp"
#include <gfx/font.hpp>
#include <memory>
#include <string>

namespace ui {
    class Label : public Widget {
    private:
        std::string m_caption;
        utils::Color m_color;
        std::shared_ptr<gfx::Font> m_font;
        int m_max_font_size;
        float m_font_size;
        std::string m_wrapped_text;
        Alignment m_alignment;
        VerticalAlignment m_vertical_alignment;
        utils::Vec2f m_text_size;

    public:
        Label(std::string caption,
              std::shared_ptr<gfx::Font> font,
              int const max_size,
              utils::Color const color,
              Alignment const alignment = Alignment::Left,
              VerticalAlignment const vertical_alignment = VerticalAlignment::Top)
            : m_caption{ std::move(caption) },
              m_color{ color },
              m_font{ std::move(font) },
              m_max_font_size{ max_size },
              m_font_size{ 1.0f },
              m_alignment{ alignment },
              m_vertical_alignment{ vertical_alignment } { }

        bool handle_event([[maybe_unused]] Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override;

    private:
        void recalculate();
        [[nodiscard]] bool does_fit(char const* text, int font_size, utils::Vec2f& out_text_size);
    };
} // namespace ui
