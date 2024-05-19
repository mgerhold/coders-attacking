#include <ui/widget.hpp>

namespace ui {

    void Widget::recalculate_absolute_area(utils::IntRect const& outer_area) {
        using namespace utils;
        m_absolute_area = IntRect{ FloatRect{ outer_area }.relative_to_absolute_rect(m_relative_area) };
    }

} // namespace ui
