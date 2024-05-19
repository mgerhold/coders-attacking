#include <cassert>
#include <lib2k/string_utils.hpp>
#include <ui/label.hpp>

namespace ui {

    void Label::render(gfx::Renderer& renderer) const {
        auto const x = std::invoke([&] {
            switch (m_alignment) {
                case Alignment::Left:
                    return absolute_area().top_left.x;
                case Alignment::Center:
                    return absolute_area().top_left.x + (absolute_area().size.x - static_cast<int>(m_text_size.x)) / 2;
                case Alignment::Right:
                    return absolute_area().top_left.x + (absolute_area().size.x - static_cast<int>(m_text_size.x));
            }
            assert(false and "unreachable");
            return 0;
        });
        auto const y = std::invoke([&] {
            switch (m_vertical_alignment) {
                case VerticalAlignment::Top:
                    return absolute_area().top_left.y;
                case VerticalAlignment::Middle:
                    return absolute_area().top_left.y + (absolute_area().size.y - static_cast<int>(m_text_size.y)) / 2;
                case VerticalAlignment::Bottom:
                    return absolute_area().top_left.y + (absolute_area().size.y - static_cast<int>(m_text_size.y));
            }
            assert(false and "unreachable");
            return 0;
        });
        renderer.draw_text(*m_font, m_wrapped_text.c_str(), utils::Vec2i{ x, y }, m_font_size, m_color);
    }

    void Label::recalculate_absolute_area(utils::IntRect const& outer_area) {
        Widget::recalculate_absolute_area(outer_area);
        recalculate_wrapping();
    }

    void Label::recalculate_wrapping() {
        auto const parts = c2k::split(m_caption, " ");
        assert(not parts.empty());

        auto font_size = m_max_font_size;
        auto last_try = std::string{};
        while (font_size > 1) {
            last_try.clear();
            auto it = parts.cbegin();
            auto line = *it;
            if (not does_fit(line.c_str(), font_size, m_text_size)) {
                --font_size;
                continue;
            }

            ++it;
            if (it == parts.cend()) {
                // there's only one word and the word fits
                m_wrapped_text = std::move(line);
                m_font_size = static_cast<float>(font_size);
                return;
            }

            auto is_start_of_line = false;
            while (true) {
                auto appended = line;
                if (not is_start_of_line) {
                    appended += ' ';
                }
                appended += *it;
                if (not does_fit(appended.c_str(), font_size, m_text_size)) {
                    if (is_start_of_line) {
                        // even the first word on this line does not fit
                        // -> reduce font size and try again from the start
                        --font_size;
                        last_try = std::move(line);
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
                        m_wrapped_text = std::move(line);
                        m_font_size = static_cast<float>(font_size);
                        return;
                    }
                }
            }
        }

        // not everything fits, but we cannot go any smaller -> trim the text so that it just fits
        m_font_size = static_cast<float>(std::max(font_size, 1));
        m_wrapped_text = std::move(last_try);
    }

    bool Label::does_fit(char const* const text, int const font_size, utils::Vec2f& out_text_size) {
        auto const max_area = utils::Vec2f{ absolute_area().size };
        auto const area = m_font->measure_text(text, static_cast<float>(font_size));
        out_text_size = area;
        return area.x <= max_area.x and area.y <= max_area.y;
    }
} // namespace ui
