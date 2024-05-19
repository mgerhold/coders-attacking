#pragma once

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
        int m_max_size;
        float m_size;
        std::string m_wrapped;

    public:
        Label(utils::FloatRect const& relative_area,
              std::string caption,
              std::shared_ptr<gfx::Font> font,
              int const max_size,
              utils::Color const color)
            : Widget{ relative_area },
              m_caption{ std::move(caption) },
              m_color{ color },
              m_font{ std::move(font) },
              m_max_size{ max_size },
              m_size{ 1.0f } { }

        bool handle_event([[maybe_unused]] Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override;

    private:
        void recalculate();
        [[nodiscard]] bool does_fit(const char* text, int font_size);
    };
} // namespace ui
