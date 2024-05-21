#include <ui/focus_manager.hpp>
#include <ui/widget.hpp>

namespace ui {
    [[nodiscard]] bool FocusManager::has_focus(Widget const& widget) const {
        if (not m_focused_widget.has_value()) {
            return false;
        }

        if (auto const id = widget.focus_id()) {
            return id.value() == m_focused_widget->focus_id();
        }

        return false;
    }
} // namespace ui
