#pragma once

#include "layout.hpp"
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
        Panel(utils::FloatRect const& relative_area,
              std::unique_ptr<Layout> layout,
              tl::optional<utils::Color> const color = tl::nullopt)
            : Widget{ relative_area },
              m_color{ color },
              m_layout{ std::move(layout) } { }

        void add_widget(std::unique_ptr<Widget> widget) {
            m_widgets.emplace_back(std::move(widget));
            m_layout->recalculate(m_widgets.size());
        }

        void clear_widgets() {
            m_widgets.clear();
            m_layout->recalculate(m_widgets.size());
        }

        [[nodiscard]] bool handle_event(Event const event) override {
            return std::ranges::any_of(m_widgets, [event](auto const& widget) { return widget->handle_event(event); });
        }

        void render(gfx::Renderer& renderer) const override;

        void recalculate_absolute_area(utils::IntRect const& outer_area) override {
            using std::ranges::views::zip;
            using namespace utils;

            Widget::recalculate_absolute_area(outer_area);
            auto const& sub_areas = m_layout->sub_areas();
            assert(sub_areas.size() == m_widgets.size());
            for (auto const& [widget, area] : zip(m_widgets, sub_areas)) {
                auto const absolute_area = IntRect{ FloatRect{ m_absolute_area }.relative_to_absolute_rect(area) };
                widget->recalculate_absolute_area(absolute_area);
            }
        }
    };
} // namespace ui
