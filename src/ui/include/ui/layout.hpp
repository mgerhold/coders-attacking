#pragma once

#include <lib2k/types.hpp>
#include <utils/rect.hpp>
#include <vector>

namespace ui {
    class Layout {
    private:
        std::vector<utils::FloatRect> m_sub_areas;

    public:
        Layout() = default;
        Layout(Layout const& other) = default;
        Layout(Layout&& other) noexcept = default;
        Layout& operator=(Layout const& other) = default;
        Layout& operator=(Layout&& other) noexcept = default;
        virtual ~Layout() = default;

        virtual void recalculate(usize num_sub_areas) = 0;

        [[nodiscard]] std::vector<utils::FloatRect> const& sub_areas() const {
            return m_sub_areas;
        }

    protected:
        void set_sub_areas(std::vector<utils::FloatRect> sub_areas) {
            m_sub_areas = std::move(sub_areas);
        }
    };
} // namespace ui
