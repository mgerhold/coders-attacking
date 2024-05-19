#pragma once

#include "layout.hpp"
#include <lib2k/types.hpp>
#include <stdexcept>
#include <vector>

namespace ui {
    class GridLayout : public Layout {
    public:
        struct Area {
            utils::Vec2i top_left;
            utils::Vec2i span;

            Area(utils::Vec2i const top_left, utils::Vec2i const span) : top_left{ top_left }, span{ span } {
                if (span.x == 0 or span.y == 0) {
                    throw std::invalid_argument{ "cannot define an empty area" };
                }
            }
        };

    private:
        usize m_width;
        usize m_height;
        std::vector<Area> m_areas;

    public:
        GridLayout(usize const width, usize const height, std::convertible_to<Area> auto&&... areas)
            : m_width{ width },
              m_height{ height } {
            static_assert(sizeof...(areas) > 0, "must define at least one area");
            if (height == 0 or width == 0) {
                throw std::invalid_argument{ "need at least one row and one column in GridLayout" };
            }

            auto const is_valid = [&](Area const& area) {
                return area.top_left.x + area.span.x <= m_width and area.top_left.y + area.span.y <= m_height;
            };

            m_areas.reserve(sizeof...(areas));
            (
                    [&]() {
                        if (not is_valid(areas)) {
                            throw std::invalid_argument{ "invalid area" };
                        }
                        m_areas.emplace_back(std::forward<decltype(areas)>(areas));
                    }(),
                    ...
            );
        }

        void recalculate(usize const num_sub_areas) override {
            using namespace utils;

            if (num_sub_areas > m_areas.size()) {
                throw std::invalid_argument{ "not enough areas defined in GridLayout" };
            }

            auto const column_width = 1.0f / static_cast<float>(m_width);
            auto const row_height = 1.0f / static_cast<float>(m_height);
            auto areas = std::vector<FloatRect>{};
            for (auto i = usize{ 0 }; i < std::min(num_sub_areas, m_areas.size()); ++i) {
                auto const top_left = Vec2f{
                    static_cast<float>(m_areas.at(i).top_left.x) / static_cast<float>(m_width),
                    static_cast<float>(m_areas.at(i).top_left.y) / static_cast<float>(m_height),
                };
                areas.emplace_back(
                        top_left,
                        Vec2f{
                                column_width * static_cast<float>(m_areas.at(i).span.x),
                                row_height * static_cast<float>(m_areas.at(i).span.y),
                        }
                );
            }

            set_sub_areas(std::move(areas));
        }
    };
} // namespace ui
