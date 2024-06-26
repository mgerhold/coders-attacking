#include "gfx/window.hpp"


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

    [[nodiscard]] HandleEventResult FocusManager::handle_event(Event const event) {
        auto const& event_system = m_service_provider->window().event_system();
        if (auto const key_pressed = std::get_if<KeyPressed>(&event)) {
            if (key_pressed->key == Key::Tab) {
                if (event_system.is_key_down(Key::LeftShift) or event_system.is_key_down(Key::RightShift)) {
                    previous();
                } else {
                    next();
                }
                return HandleEventResult::EventHandled;
            }
        }
        return HandleEventResult::EventNotHandled;
    }

    void FocusManager::next() {
        if (not m_focused_widget.has_value()) {
            auto_focus();
            return;
        }

        auto focusable_widgets = std::vector<Widget*>{};
        m_root->collect_focusable_widgets(focusable_widgets);
        std::ranges::sort(focusable_widgets, [](Widget const* const lhs, Widget const* const rhs) {
            return lhs->focus_id().value() < rhs->focus_id().value();
        });
        auto find_it = std::ranges::find(focusable_widgets, &m_focused_widget.value());
        if (find_it == focusable_widgets.end()) {
            auto_focus();
        }
        ++find_it;
        auto const successor = (find_it == focusable_widgets.end() ? focusable_widgets.begin() : find_it);
        focus(**successor);
    }

    void FocusManager::previous() {
        if (not m_focused_widget.has_value()) {
            auto_focus();
            return;
        }

        auto focusable_widgets = std::vector<Widget*>{};
        m_root->collect_focusable_widgets(focusable_widgets);
        std::ranges::sort(focusable_widgets, [](Widget const* const lhs, Widget const* const rhs) {
            return lhs->focus_id().value() < rhs->focus_id().value();
        });
        auto const find_it = std::ranges::find(focusable_widgets, &m_focused_widget.value());
        if (find_it == focusable_widgets.end()) {
            auto_focus();
        }
        auto const predecessor = (find_it == focusable_widgets.begin() ? focusable_widgets.end() - 1 : find_it - 1);
        focus(**predecessor);
    }
} // namespace ui
