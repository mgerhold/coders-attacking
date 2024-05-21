#pragma once

#include "event.hpp"
#include <algorithm>
#include <gfx/renderer.hpp>
#include <tl/optional.hpp>
#include <vector>
#include <utils/rect.hpp>
#include <utils/vec2.hpp>

namespace ui {
    class FocusManager;

    class Widget {
    private:
        utils::IntRect m_area;
        std::string m_name;
        FocusManager* m_focus_manager{ nullptr };

    protected:
        [[nodiscard]] utils::IntRect area() const {
            return m_area;
        }

        [[nodiscard]] FocusManager& focus_manager() const {
            if (m_focus_manager == nullptr) {
                throw std::runtime_error{ "no focus manager registered" };
            }
            return *m_focus_manager;
        }

    public:
        explicit Widget(std::string name = "") : m_name{ std::move(name) } { }
        Widget(Widget const& other) = default;
        Widget(Widget&& other) noexcept = default;
        Widget& operator=(Widget const& other) = default;
        Widget& operator=(Widget&& other) noexcept = default;
        virtual ~Widget() = default;

        void name(std::string name) {
            m_name = std::move(name);
        }

        [[nodiscard]] std::string const& name() const {
            return m_name;
        }

        [[nodiscard]] virtual HandleEventResult handle_event(Event event) = 0;
        virtual void render(gfx::Renderer& renderer) const = 0;
        virtual void recalculate_absolute_area(utils::IntRect const& outer_area);

        virtual void register_focus_manager(FocusManager& focus_manager) {
            m_focus_manager = &focus_manager;
        }

        virtual void collect_focusable_widgets([[maybe_unused]] std::vector<Widget*>& focusable_widgets) { }

        [[nodiscard]] virtual tl::optional<u32> focus_id() const {
            return tl::nullopt;
        }

        virtual void on_focused() { }

        virtual void on_unfocused() { }

        [[nodiscard]] virtual tl::optional<Widget&> find_widget_by_name(std::string_view const name) {
            if (this->name() == name) {
                return *this;
            }
            return tl::nullopt;
        }

        [[nodiscard]] virtual tl::optional<Widget const&> find_widget_by_name(std::string_view name) const {
            if (this->name() == name) {
                return *this;
            }
            return tl::nullopt;
        }
    };
} // namespace ui
