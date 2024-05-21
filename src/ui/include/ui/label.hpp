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
        std::vector<std::string> m_wrapped_text;
        Alignment m_alignment;
        VerticalAlignment m_vertical_alignment;
        float m_line_height;
        utils::Vec2f m_text_size;

    public:
        Label(std::string caption,
              std::shared_ptr<gfx::Font> font,
              int const max_size,
              utils::Color const color,
              Alignment const alignment = Alignment::Left,
              VerticalAlignment const vertical_alignment = VerticalAlignment::Top,
              float const line_height = 1.2f)
            : Label{
                  {}, std::move(caption), std::move(font), max_size, color, alignment, vertical_alignment, line_height,
              } { }

        Label(WidgetName&& widget_name,
              std::string caption,
              std::shared_ptr<gfx::Font> font,
              int const max_size,
              utils::Color const color,
              Alignment const alignment = Alignment::Left,
              VerticalAlignment const vertical_alignment = VerticalAlignment::Top,
              float const line_height = 1.2f)
            : Widget{ std::move(widget_name) },
              m_caption{ std::move(caption) },
              m_color{ color },
              m_font{ std::move(font) },
              m_max_font_size{ max_size },
              m_font_size{ 1.0f },
              m_alignment{ alignment },
              m_vertical_alignment{ vertical_alignment },
              m_line_height{ line_height } {
            if (line_height < 0.5f) {
                throw std::invalid_argument{ "line height must be at least 0.5" };
            }
        }

        HandleEventResult handle_event([[maybe_unused]] Event event) override {
            return HandleEventResult::EventNotHandled;
        }

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override;

        [[nodiscard]] std::string const& caption() const {
            return m_caption;
        }

        void caption(std::string caption) {
            m_caption = std::move(caption);
            recalculate_wrapping();
        }

    private:
        void recalculate_wrapping();
        [[nodiscard]] bool does_fit(std::vector<std::string> lines, int font_size, utils::Vec2f& out_text_size);
    };
} // namespace ui
