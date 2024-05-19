#pragma once

#include "layout.hpp"
#include "ui/bumper.hpp"
#include "ui/column_layout.hpp"
#include "widget.hpp"
#include <algorithm>
#include <cassert>
#include <memory>
#include <ranges>
#include <tl/optional.hpp>
#include <utils/color.hpp>

namespace ui {
    class Panel : public Widget {
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
                auto const sub_area = IntRect{ FloatRect{ this->area() }.relative_to_absolute_rect(area) };
                widget->recalculate_absolute_area(sub_area);
            }
        }
    };
} // namespace ui
