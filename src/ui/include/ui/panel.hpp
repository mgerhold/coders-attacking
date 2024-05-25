#pragma once

#include "bumper.hpp"
#include "column_layout.hpp"
#include "focus_manager.hpp"
#include "layout.hpp"
#include "widget.hpp"
#include <algorithm>
#include <cassert>
#include <memory>
#include <ranges>
#include <tl/optional.hpp>
#include <utils/color.hpp>

namespace ui {
    class Panel final : public Widget {
    private:
        tl::optional<utils::Color> m_color;
        std::unique_ptr<Layout> m_layout;
        std::vector<std::unique_ptr<Widget>> m_widgets;

    public:
        explicit Panel(tl::optional<utils::Color> const color = tl::nullopt)
            : Panel{ std::make_unique<ColumnLayout>(1), color } { }

        explicit Panel(std::unique_ptr<Layout> layout, tl::optional<utils::Color> const color = tl::nullopt)
            : m_color{ color },
              m_layout{ std::move(layout) } { }

        template<usize prepended_bumpers = 0, std::convertible_to<std::unique_ptr<Widget>>... Widgets>
        void add_widgets(Widgets&&... widget) {
            for (auto i = usize{ 0 }; i < prepended_bumpers; ++i) {
                m_widgets.emplace_back(std::make_unique<ui::Bumper>());
            }
            (([&]() { m_widgets.emplace_back(std::forward<Widgets>(widget)); }()), ...);
            m_layout->recalculate(m_widgets.size());
        }

        void clear_widgets() {
            m_widgets.clear();
            m_layout->recalculate(m_widgets.size());
        }

        [[nodiscard]] HandleEventResult handle_event(Event const event) override {
            for (auto const& widget : m_widgets) {
                if (widget->handle_event(event) == HandleEventResult::EventHandled) {
                    return HandleEventResult::EventHandled;
                }
            }
            return HandleEventResult::EventNotHandled;
        }

        void set_layout(std::unique_ptr<Layout> layout) {
            m_layout = std::move(layout);
            m_layout->recalculate(m_widgets.size());
            recalculate_absolute_area(area());
        }

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override {
            using std::ranges::views::zip;
            using namespace utils;

            Widget::recalculate_absolute_area(outer_area);
            auto const& sub_areas = m_layout->sub_areas();
            assert(sub_areas.size() == m_widgets.size());
            for (auto const& [widget, area] : zip(m_widgets, sub_areas)) {
                auto const sub_area = FloatRect{ this->area() }.relative_to_absolute_rect(area).round();
                widget->recalculate_absolute_area(sub_area);
            }
        }

        void register_focus_manager(FocusManager& focus_manager) override {
            Widget::register_focus_manager(focus_manager);
            for (auto const& widget : m_widgets) {
                widget->register_focus_manager(focus_manager);
            }
        }

        void collect_focusable_widgets(std::vector<Widget*>& focusable_widgets) override {
            // the panel itself is not focusable, but maybe its children
            for (auto const& widget : m_widgets) {
                widget->collect_focusable_widgets(focusable_widgets);
            }
        }

        [[nodiscard]] tl::optional<Widget&> find_widget_by_name(std::string_view name) override {
            if (auto const result = Widget::find_widget_by_name(name)) {
                return result;
            }
            for (auto const& widget : m_widgets) {
                if (auto result = widget->find_widget_by_name(name)) {
                    return result.value();
                }
            }
            return tl::nullopt;
        }

        [[nodiscard]] tl::optional<Widget const&> find_widget_by_name(std::string_view const name) const override {
            if (auto const result = Widget::find_widget_by_name(name)) {
                return result;
            }
            for (auto const& widget : m_widgets) {
                if (auto const result = widget->find_widget_by_name(name)) {
                    return result.value();
                }
            }
            return tl::nullopt;
        }
    };
} // namespace ui
