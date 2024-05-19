#include <ui/panel.hpp>

namespace ui {
    void Panel::render(gfx::Renderer& renderer) const {
        // render background (if a color was set)
        if (m_color.has_value()) {
            renderer.draw_filled_rectangle(m_absolute_area, m_color.value());
        }

        // render sub-widgets
        for (auto const& widget : m_widgets) {
            widget->render(renderer);
        }
    }
} // namespace ui
