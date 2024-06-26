#include <ui/widget.hpp>

namespace ui {

    void Widget::recalculate_absolute_area(utils::IntRect const& outer_area) {
        using namespace utils;
        m_area = IntRect{ FloatRect{ outer_area }.relative_to_absolute_rect(FloatRect::unit()) };
    }

} // namespace ui
