#pragma once

#include "event_system.hpp"
#include "widget.hpp"
#include <common/service_provider.hpp>
#include <lib2k/types.hpp>
#include <tl/optional.hpp>

namespace ui {
    class Widget;

    class FocusManager final {
    private:
        ServiceProvider* m_service_provider;
        tl::optional<Widget&> m_focused_widget;
        Widget* m_root;

    public:
        explicit FocusManager(ServiceProvider& service_provider, Widget& root)
            : m_service_provider{ &service_provider },
              m_root{ &root } {
            m_root->register_focus_manager(*this);
            auto_focus();
        }

        [[nodiscard]] bool has_focus(Widget const& widget) const;

        void focus(Widget& widget) {
            unfocus();
            m_focused_widget = widget;
            m_focused_widget->on_focused();
        }

        void unfocus() {
            if (m_focused_widget.has_value()) {
                m_focused_widget->on_unfocused();
            }
            m_focused_widget.reset();
        }

        void unfocus(Widget const& widget) {
            if (m_focused_widget.has_value() and m_focused_widget->focus_id() == widget.focus_id().value()) {
                unfocus();
            }
        }

        void auto_focus() {
            unfocus();
            auto focusable_widgets = std::vector<Widget*>{};
            m_root->collect_focusable_widgets(focusable_widgets);
            auto const min_id = std::ranges::min_element(focusable_widgets, [](auto const& lhs, auto const& rhs) {
                return lhs->focus_id().value() < rhs->focus_id().value();
            });
            if (min_id != focusable_widgets.end()) {
                focus(**min_id);
            }
        }

        [[nodiscard]] HandleEventResult handle_event(Event event);

    private:
        void next();
        void previous();
    };
} // namespace ui
