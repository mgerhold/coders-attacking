#include <ui/label.hpp>

namespace ui {
    Label::Label(utils::FloatRect const position, std::string caption, utils::Color const color)
        : Widget{ position },
          m_caption{ std::move(caption) },
          m_color{ color } { }

    void Label::render(gfx::Renderer& renderer) const {
        renderer.draw_text(m_caption.c_str(), m_absolute_area.top_left, 20, m_color);
    }
} // namespace ui
