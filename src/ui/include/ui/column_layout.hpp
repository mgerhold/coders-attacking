#pragma once

#include "layout.hpp"
#include <stdexcept>
#include <utils/math.hpp>

namespace ui {
    class ColumnLayout : public Layout {
    private:
        usize m_num_columns;

    public:
        explicit ColumnLayout(usize const num_columns) : m_num_columns{ num_columns } {
            if (num_columns == 0) {
                throw std::invalid_argument{ "need at least one column in ColumnLayout" };
            }
        }

        void recalculate(usize const num_sub_areas) override {
            using namespace utils;

            if (num_sub_areas == 0) {
                set_sub_areas({});
            }
            auto const num_rows = ceil_div(num_sub_areas, m_num_columns);
            auto const column_width = 1.0f / static_cast<float>(m_num_columns);
            auto const row_height = 1.0f / static_cast<float>(num_rows);

            auto areas = std::vector<FloatRect>{};
            for (auto row = usize{ 0 }, sub_area = usize{ 0 }; row < num_rows and sub_area < num_sub_areas; ++row) {
                for (auto column = usize{ 0 }; column < m_num_columns and sub_area < num_sub_areas;
                     ++column, ++sub_area) {
                    auto const top_left = Vec2f{
                        static_cast<float>(column) * column_width,
                        static_cast<float>(row) * row_height,
                    };
                    areas.emplace_back(top_left, Vec2f{ column_width, row_height });
                }
            }
            set_sub_areas(std::move(areas));
        }
    };
} // namespace ui
