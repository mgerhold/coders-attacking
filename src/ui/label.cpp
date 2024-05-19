#include <cassert>
#include <lib2k/string_utils.hpp>
#include <ui/label.hpp>

namespace ui {

    void Label::render(gfx::Renderer& renderer) const {
        renderer.draw_text(*m_font, m_wrapped.c_str(), m_absolute_area.top_left, m_size, m_color);
    }

    void Label::recalculate_absolute_area(utils::IntRect const& outer_area) {
        Widget::recalculate_absolute_area(outer_area);
        recalculate();
    }

    void Label::recalculate() {
        auto const parts = c2k::split(m_caption, " ");
        assert(not parts.empty());

        auto font_size = m_max_size;
        while (font_size > 1) {
            auto it = parts.cbegin();
            auto line = *it;
            if (not does_fit(line.c_str(), font_size)) {
                --font_size;
                continue;
            }

            ++it;
            auto is_start_of_line = false;
            while (true) {
                auto appended = line;
                if (not is_start_of_line) {
                    appended += ' ';
                }
                appended += *it;
                if (not does_fit(appended.c_str(), font_size)) {
                    if (is_start_of_line) {
                        // even the first word on this line does not fit
                        // -> reduce font size and try again from the start
                        --font_size;
                        break;
                    }
                    // the current word does not fit, but it's not the first word
                    // -> wrap line instead of appending the word
                    line += '\n';
                    is_start_of_line = true;
                } else {
                    // current word can be appended and everything still fits
                    line = std::move(appended);
                    is_start_of_line = false;
                    ++it;

                    if (it == parts.cend()) { // all words processed, everything fits
                        m_wrapped = std::move(line);
                        m_size = static_cast<float>(font_size);
                        return;
                    }
                }
            }
        }
        // not everything fits, but we cannot go any smaller
        m_size = static_cast<float>(std::max(font_size, 1));
    }

    bool Label::does_fit(char const* const text, int const font_size) {
        auto const max_area = utils::Vec2f{ m_absolute_area.size };
        auto const area = m_font->measure_text(text, static_cast<float>(font_size));
        return area.x <= max_area.x and area.y <= max_area.y;
    }
} // namespace ui
